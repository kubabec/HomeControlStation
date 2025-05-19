#ifndef TEMP_GAUGE_JS_H
#define TEMP_GAUGE_JS_H


const char* tempGaugeJS = "\
<script>\
        const gaugeRegistry = {};\
        var listOfTempWidgets = [];\
        var listOfTempValues = [];\
        var listOfHumidWidgets = [];\
        var listOfHumidValues = [];\
\
        class TemperatureGauge {\
            constructor(containerId, minTemp = -10, maxTemp = 40) {\
                this.container = document.getElementById(containerId);\
                this.canvas = this.container.querySelector('canvas');\
                this.valueDisplay = this.container.querySelector('.temperature-value');\
                this.minTemp = minTemp;\
                this.maxTemp = maxTemp;\
                this.currentTemp = minTemp;\
                this.ctx = this.canvas.getContext('2d');\
                this.initCanvas();\
                this.draw();\
            }\
\
            initCanvas() {\
                const dpr = window.devicePixelRatio || 1;\
                const rect = this.canvas.getBoundingClientRect();\
                this.canvas.width = rect.width * dpr;\
                this.canvas.height = rect.height * dpr;\
                this.ctx.scale(dpr, dpr);\
            }\
\
            draw() {\
                const ctx = this.ctx;\
                const width = this.canvas.width / (window.devicePixelRatio || 1);\
                const height = this.canvas.height / (window.devicePixelRatio || 1);\
                const centerX = width / 2;\
                const centerY = height / 2;\
                const radius = Math.min(width, height) * 0.4;\
\
                ctx.clearRect(0, 0, width, height);\
\
                let gradient = ctx.createLinearGradient(centerX - radius, centerY, centerX + radius, centerY);\
                gradient.addColorStop(0, 'blue');\
                gradient.addColorStop(0.35, 'orange');\
                gradient.addColorStop(1, 'red');\
\
                ctx.beginPath();\
                ctx.arc(centerX, centerY, radius, -Math.PI * 1.25, Math.PI * 0.25);\
                ctx.strokeStyle = gradient;\
                ctx.lineWidth = 8;\
                ctx.stroke();\
\
                const scaleRange = this.maxTemp - this.minTemp;\
                const angleRange = 1.5 * Math.PI;\
                const tempAngle = -Math.PI * 0.75 + ((this.currentTemp - this.minTemp) / scaleRange) * angleRange;\
                const arrowLength = radius * 0.8;\
\
                ctx.save();\
                ctx.translate(centerX, centerY);\
                ctx.rotate(tempAngle);\
\
                ctx.beginPath();\
                ctx.moveTo(0, 0);\
                ctx.lineTo(0, -arrowLength);\
                ctx.strokeStyle = '#5A8DEE';\
                ctx.lineWidth = 3;\
                ctx.stroke();\
\
                ctx.restore();\
\
                ctx.beginPath();\
                ctx.arc(centerX, centerY, 5, 0, Math.PI * 2);\
                ctx.fillStyle = '#5A8DEE';\
                ctx.fill();\
\
                this.drawLabels(ctx, centerX, centerY, radius);\
            }\
\
            drawLabels(ctx, centerX, centerY, radius) {\
                ctx.fillStyle = '#678cd2';\
                ctx.font = '13px Arial';\
                ctx.textAlign = 'center';\
                ctx.textBaseline = 'middle';\
\
                ctx.fillText('-10°', centerX - radius-3, centerY + 37);\
                ctx.fillText('40°', centerX + radius +1, centerY + 37);\
                ctx.fillText('15°', centerX+2, centerY - radius - 9);\
            }\
\
            update(temp) {\
                this.currentTemp = Math.max(this.minTemp, Math.min(this.maxTemp, temp));\
                this.valueDisplay.textContent = `${this.currentTemp}°C`;\
                this.draw();\
            }\
        }\
\
        class HumidityGauge {\
            constructor(containerId) {\
                this.container = document.getElementById(containerId);\
                this.canvas = this.container.querySelector('canvas');\
                this.valueDisplay = this.container.querySelector('.humidity-value');\
                this.ctx = this.canvas.getContext('2d');\
                this.humidity = 50;\
                this.initCanvas();\
                this.draw();\
            }\
\
            initCanvas() {\
                const dpr = window.devicePixelRatio || 1;\
                const rect = this.canvas.getBoundingClientRect();\
                this.canvas.width = rect.width * dpr;\
                this.canvas.height = rect.height * dpr;\
                this.ctx.scale(dpr, dpr);\
            }\
\
            draw() {\
                const ctx = this.ctx;\
                const width = this.canvas.width / (window.devicePixelRatio || 1);\
                const height = this.canvas.height / (window.devicePixelRatio || 1);\
                ctx.clearRect(0, 0, width, height);\
\
                ctx.strokeStyle = '#ccc';\
                ctx.lineWidth = 1;\
                ctx.fillStyle = '#678cd2';\
                ctx.font = '11px Arial';\
                for (let i = 0; i <= 4; i++) {\
                    let y = height - (i * height / 4);\
                    ctx.beginPath();\
                    ctx.moveTo(width / 4, y);\
                    ctx.lineTo((3 * width) / 4, y);\
                    ctx.stroke();\
                    \
                    if(i == 0){\
                        y -= 5;\
                    }else if(i == 4){\
                        y += 5;\
                    }\
                    ctx.fillText(`${i * 25}%`, (3 * width) / 4 + 5, y + 3);\
                }\
\
                const barHeight = (this.humidity / 100) * height;\
                ctx.fillStyle = `rgba(0, 0, 255, ${this.humidity / 100})`;\
                ctx.fillRect(width / 3, height - barHeight, width / 3, barHeight);\
            }\
\
            update(humidity) {\
                this.humidity = Math.max(0, Math.min(100, humidity));\
                this.valueDisplay.textContent = `${this.humidity}%`;\
                this.draw();\
            }\
        }\
\
        function createGauge(containerId, minTemp = -10, maxTemp = 40) {\
            gaugeRegistry[containerId] = new TemperatureGauge(containerId, minTemp, maxTemp);\
        }\
\
        function createHumidGauge(containerId) {\
            gaugeRegistry[containerId] = new HumidityGauge(containerId);\
        }\
\
        function setHumidity(containerId, humidity) {\
            if (gaugeRegistry[containerId]) {\
                gaugeRegistry[containerId].update(humidity);\
            }\
        }\
\
        function setTemperature(containerId, temperature) {\
            if (gaugeRegistry[containerId]) {\
                gaugeRegistry[containerId].update(temperature);\
            } else {\
                console.error(`Gauge with ID ${containerId} not found`);\
            }\
        }\
        </script>\
";


#endif