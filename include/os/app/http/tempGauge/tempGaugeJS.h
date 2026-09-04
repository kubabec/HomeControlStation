#ifndef TEMP_GAUGE_JS_H
#define TEMP_GAUGE_JS_H

/**
 * @file include/os/app/http/tempGauge/tempGaugeJS.h
 * @brief Embedded web UI asset used by the Home Control Station HTTP interface.
 */



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
                this.valueDisplay = this.container.querySelector('.temperature-value');\
                this.fill = this.container.querySelector('.temperature-fill');\
                this.marker = this.container.querySelector('.temperature-marker');\
                this.minTemp = minTemp;\
                this.maxTemp = maxTemp;\
            }\
\
            update(temp) {\
                const currentTemp = Math.max(this.minTemp, Math.min(this.maxTemp, Number(temp)));\
                const progress = ((currentTemp - this.minTemp) / (this.maxTemp - this.minTemp)) * 100;\
                this.valueDisplay.textContent = `${Number(temp).toFixed(1)}°C`;\
                this.fill.style.width = `${progress}%`;\
                this.marker.style.left = `${progress}%`;\
            }\
        }\
\
        class HumidityGauge {\
            constructor(containerId) {\
                this.container = document.getElementById(containerId);\
                this.valueDisplay = this.container.querySelector('.humidity-value');\
                this.fill = this.container.querySelector('.humidity-fill');\
            }\
\
            update(humidity) {\
                const currentHumidity = Math.max(0, Math.min(100, Number(humidity)));\
                this.valueDisplay.textContent = `${currentHumidity.toFixed(0)}%`;\
                this.fill.style.width = `${currentHumidity}%`;\
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