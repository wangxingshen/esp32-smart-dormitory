const REFRESH_INTERVAL_MS = 3000;

const state = {
    autoRefresh: true,
    alarmOnlyUnhandled: false,
    timer: null,
    history: [],
};

const el = {
    deviceIdInput: document.getElementById("deviceIdInput"),
    refreshButton: document.getElementById("refreshButton"),
    autoRefreshToggle: document.getElementById("autoRefreshToggle"),
    onlyUnhandledButton: document.getElementById("onlyUnhandledButton"),
    chartField: document.getElementById("chartField"),
    chart: document.getElementById("historyChart"),
    apiState: document.getElementById("apiState"),
    refreshTime: document.getElementById("refreshTime"),
    dataTime: document.getElementById("dataTime"),
    deviceStatusDot: document.getElementById("deviceStatusDot"),
    deviceStatusText: document.getElementById("deviceStatusText"),
    deviceLastSeen: document.getElementById("deviceLastSeen"),
    temperature: document.getElementById("temperature"),
    humidity: document.getElementById("humidity"),
    light: document.getElementById("light"),
    smoke: document.getElementById("smoke"),
    controlState: document.getElementById("controlState"),
    alarmList: document.getElementById("alarmList"),
};

function deviceId() {
    return el.deviceIdInput.value.trim() || "esp32_001";
}

async function apiGet(url) {
    const response = await fetch(url);
    if (!response.ok) {
        throw new Error(`HTTP ${response.status}`);
    }
    const body = await response.json();
    if (body.code !== 200) {
        throw new Error(body.message || "接口返回失败");
    }
    return body.data;
}

async function apiPost(url, payload) {
    const response = await fetch(url, {
        method: "POST",
        headers: {"Content-Type": "application/json"},
        body: JSON.stringify(payload),
    });
    if (!response.ok) {
        throw new Error(`HTTP ${response.status}`);
    }
    const body = await response.json();
    if (body.code !== 200) {
        throw new Error(body.message || "接口返回失败");
    }
    return body.data;
}

async function refreshDashboard() {
    const id = encodeURIComponent(deviceId());
    const refreshStartedAt = new Date();
    el.apiState.textContent = "刷新中";

    try {
        const [latest, history, status, alarms] = await Promise.all([
            apiGet(`/api/device/latest?deviceId=${id}`),
            apiGet(`/api/device/history?deviceId=${id}&limit=60`),
            apiGet(`/api/device/status?deviceId=${id}`),
            apiGet(`/api/alarm/list${state.alarmOnlyUnhandled ? "?handled=false" : ""}`),
        ]);

        updateLatest(latest);
        updateStatus(status);
        updateHistory(history || []);
        updateAlarms(alarms || []);

        el.refreshTime.textContent = formatDateTime(refreshStartedAt);
        el.apiState.textContent = "已同步";
    } catch (error) {
        el.apiState.textContent = `失败：${error.message}`;
        el.refreshTime.textContent = formatDateTime(new Date());
    }
}

function updateLatest(data) {
    if (!data) {
        el.dataTime.textContent = "暂无数据";
        setMetric("--", "--", "--", "--");
        return;
    }

    el.dataTime.textContent = `${formatDateTime(data.createdAt)}，${relativeTime(data.createdAt)}`;
    setMetric(
        formatNumber(data.temperature, 1),
        formatNumber(data.humidity, 1),
        formatNumber(data.light, 0),
        formatNumber(data.smoke, 0),
    );
}

function setMetric(temperature, humidity, light, smoke) {
    el.temperature.textContent = temperature;
    el.humidity.textContent = humidity;
    el.light.textContent = light;
    el.smoke.textContent = smoke;
}

function updateStatus(status) {
    el.deviceStatusDot.classList.remove("online", "offline");

    if (!status) {
        el.deviceStatusText.textContent = "未知";
        el.deviceLastSeen.textContent = "--";
        return;
    }

    el.deviceStatusText.textContent = status.online ? "在线" : "离线";
    el.deviceStatusDot.classList.add(status.online ? "online" : "offline");
    el.deviceLastSeen.textContent = status.lastSeen
        ? `最后在线 ${relativeTime(status.lastSeen)}`
        : "无在线记录";
}

function updateHistory(history) {
    state.history = history.slice().reverse();
    drawChart();
}

function updateAlarms(alarms) {
    if (alarms.length === 0) {
        el.alarmList.innerHTML = `<div class="empty">暂无报警记录</div>`;
        return;
    }

    el.alarmList.innerHTML = alarms.slice(0, 20).map((alarm) => {
        const level = alarm.alarmLevel || "info";
        const handled = alarm.isHandled ? "已处理" : "未处理";
        return `
            <div class="alarm-item ${level}">
                <strong>${alarm.alarmMessage || alarm.alarmType || "报警"}</strong>
                <span>${alarm.deviceId} · ${handled} · ${formatDateTime(alarm.createdAt)}</span>
            </div>
        `;
    }).join("");
}

function drawChart() {
    const canvas = el.chart;
    const context = canvas.getContext("2d");
    const rect = canvas.getBoundingClientRect();
    const dpr = window.devicePixelRatio || 1;
    canvas.width = Math.max(1, Math.floor(rect.width * dpr));
    canvas.height = Math.max(1, Math.floor(rect.height * dpr));
    context.scale(dpr, dpr);

    const width = rect.width;
    const height = rect.height;
    context.clearRect(0, 0, width, height);

    const padding = {left: 44, right: 18, top: 22, bottom: 36};
    const plotWidth = width - padding.left - padding.right;
    const plotHeight = height - padding.top - padding.bottom;

    context.strokeStyle = "#dce2dd";
    context.lineWidth = 1;
    context.beginPath();
    context.moveTo(padding.left, padding.top);
    context.lineTo(padding.left, height - padding.bottom);
    context.lineTo(width - padding.right, height - padding.bottom);
    context.stroke();

    const field = el.chartField.value;
    const points = state.history
        .map((item) => ({time: item.createdAt, value: Number(item[field])}))
        .filter((item) => Number.isFinite(item.value));

    if (points.length === 0) {
        context.fillStyle = "#677270";
        context.textAlign = "center";
        context.fillText("暂无历史数据", width / 2, height / 2);
        return;
    }

    const values = points.map((item) => item.value);
    let min = Math.min(...values);
    let max = Math.max(...values);
    if (min === max) {
        min -= 1;
        max += 1;
    }
    const range = max - min;

    context.fillStyle = "#677270";
    context.textAlign = "right";
    context.fillText(max.toFixed(1), padding.left - 8, padding.top + 4);
    context.fillText(min.toFixed(1), padding.left - 8, height - padding.bottom);

    context.strokeStyle = chartColor(field);
    context.lineWidth = 2;
    context.beginPath();
    points.forEach((point, index) => {
        const x = padding.left + (points.length === 1 ? plotWidth : (plotWidth * index) / (points.length - 1));
        const y = padding.top + plotHeight - ((point.value - min) / range) * plotHeight;
        if (index === 0) {
            context.moveTo(x, y);
        } else {
            context.lineTo(x, y);
        }
    });
    context.stroke();

    context.fillStyle = chartColor(field);
    points.forEach((point, index) => {
        const x = padding.left + (points.length === 1 ? plotWidth : (plotWidth * index) / (points.length - 1));
        const y = padding.top + plotHeight - ((point.value - min) / range) * plotHeight;
        context.beginPath();
        context.arc(x, y, 3, 0, Math.PI * 2);
        context.fill();
    });

    const first = points[0];
    const last = points[points.length - 1];
    context.fillStyle = "#677270";
    context.textAlign = "left";
    context.fillText(formatTime(first.time), padding.left, height - 12);
    context.textAlign = "right";
    context.fillText(formatTime(last.time), width - padding.right, height - 12);
}

function chartColor(field) {
    return {
        temperature: "#c6533f",
        humidity: "#3a6ea5",
        light: "#c58a1b",
        smoke: "#167c80",
    }[field] || "#167c80";
}

async function sendCommand(commandType, commandValue) {
    el.controlState.textContent = "发送中";
    try {
        await apiPost("/api/device/control", {
            deviceId: deviceId(),
            commandType,
            commandValue,
        });
        el.controlState.textContent = "已下发";
        setTimeout(() => {
            el.controlState.textContent = "就绪";
        }, 1200);
    } catch (error) {
        el.controlState.textContent = `失败：${error.message}`;
    }
}

function formatNumber(value, digits) {
    const number = Number(value);
    return Number.isFinite(number) ? number.toFixed(digits) : "--";
}

function formatDateTime(value) {
    if (!value) {
        return "--";
    }
    const date = value instanceof Date ? value : new Date(value);
    if (Number.isNaN(date.getTime())) {
        return "--";
    }
    return date.toLocaleString("zh-CN", {hour12: false});
}

function formatTime(value) {
    const date = new Date(value);
    if (Number.isNaN(date.getTime())) {
        return "--";
    }
    return date.toLocaleTimeString("zh-CN", {hour12: false});
}

function relativeTime(value) {
    const date = new Date(value);
    if (Number.isNaN(date.getTime())) {
        return "--";
    }
    const seconds = Math.max(0, Math.floor((Date.now() - date.getTime()) / 1000));
    if (seconds < 60) {
        return `${seconds} 秒前`;
    }
    const minutes = Math.floor(seconds / 60);
    if (minutes < 60) {
        return `${minutes} 分钟前`;
    }
    return `${Math.floor(minutes / 60)} 小时前`;
}

function startTimer() {
    stopTimer();
    state.timer = setInterval(() => {
        if (state.autoRefresh) {
            refreshDashboard();
        }
    }, REFRESH_INTERVAL_MS);
}

function stopTimer() {
    if (state.timer) {
        clearInterval(state.timer);
    }
}

el.refreshButton.addEventListener("click", refreshDashboard);
el.autoRefreshToggle.addEventListener("change", (event) => {
    state.autoRefresh = event.target.checked;
});
el.deviceIdInput.addEventListener("change", refreshDashboard);
el.chartField.addEventListener("change", drawChart);
el.onlyUnhandledButton.addEventListener("click", () => {
    state.alarmOnlyUnhandled = !state.alarmOnlyUnhandled;
    el.onlyUnhandledButton.textContent = state.alarmOnlyUnhandled ? "全部" : "未处理";
    refreshDashboard();
});
document.querySelectorAll("[data-command-type]").forEach((button) => {
    button.addEventListener("click", () => {
        sendCommand(button.dataset.commandType, button.dataset.commandValue);
    });
});
window.addEventListener("resize", drawChart);

refreshDashboard();
startTimer();
