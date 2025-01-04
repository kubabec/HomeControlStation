#ifndef STYLE_H
#define STYLE_H


const char* style_css = " \
<style>\
body {\
    display: flex;\
    justify-content: center;\
    align-items: flex-start;\
    height: 100vh;\
    margin: 0;\
    background: #f0f8ff;\
    font-family: -apple-system, BlinkMacSystemFont, \"Segoe UI\", Roboto, \"Helvetica Neue\", Arial, sans-serif;\
    overflow-y: auto;\
    padding-top: 20px;\
    opacity: 0;\
    animation: fadeInBody 1s forwards;\
}\
.wrapper {\
    display: flex;\
    flex-direction: column;\
    align-items: center;\
    width: 100%;\
}\
.container {\
    display: flex;\
    flex-direction: column;\
    padding: 20px;\
    background: #fff;\
    border-radius: 15px;\
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\
    margin-bottom: 20px;\
    width: 300px;\
    opacity: 0;\
    transform: translateY(20px);\
    animation: fadeInSlide 1s forwards;\
}\
.header {\
    font-size: 18px;\
    font-weight: 500;\
    margin-bottom: 20px;\
    text-align: center;\
}\
.header2{\
    font-size: 15px;\
    font-weight: 400;\
    margin-bottom: 5px;\
    text-align: center;\
}\
label {\
    margin: 10px 0;\
    font-size: 14px;\
    display: flex;\
    justify-content: space-between;\
    align-items: center;\
}\
input[type=\"text\"], select {\
    padding: 10px;\
    margin-top: 5px;\
    border: 1px solid #ccc;\
    border-radius: 25px;\
    background: linear-gradient(135deg, #f0faff, #d0eaff);\
    font-size: 14px;\
    transition: background 0.3s;\
    font-family: inherit;\
    width: 200px;\
}\
input[type=\"text\"]:focus, select:focus {\
    background: linear-gradient(135deg, #e0f7ff, #c0e3ff);\
    outline: none;\
}\
.button-link, .button {\
    margin-top: 20px;\
    padding: 10px 20px;\
    background: linear-gradient(135deg, #6ec1e4, #3a8bd6);\
    border: none;\
    border-radius: 25px;\
    color: white;\
    text-align: center;\
    text-decoration: none;\
    font-size: 16px;\
    cursor: pointer;\
    transition: background 0.3s;\
    font-family: inherit;\
}\
.button-link:hover, .button:hover {\
    background: linear-gradient(135deg, #5aa9d4, #337bbf);\
}\
input[type=\"range\"] {\
    -webkit-appearance: none;\
    width: 100%;\
    height: 15px;\
    border-radius: 25px;\
    background: linear-gradient(135deg, #f0faff, #d0eaff);\
    outline: none;\
    transition: background 0.3s;\
}\
input[type=\"range\"]::-webkit-slider-thumb {\
    -webkit-appearance: none;\
    appearance: none;\
    width: 25px;\
    height: 25px;\
    border-radius: 50%;\
    background: linear-gradient(135deg, #6ec1e4, #3a8bd6);\
    cursor: pointer;\
    transition: background 0.3s;\
}\
input[type=\"range\"]::-moz-range-thumb {\
    width: 25px;\
    height: 25px;\
    border-radius: 50%;\
    background: linear-gradient(135deg, #6ec1e4, #3a8bd6);\
    cursor: pointer;\
    transition: background 0.3s;\
}\
input[type=\"range\"]:focus {\
    background: linear-gradient(135deg, #e0f7ff, #c0e3ff);\
}\
@keyframes fadeInBody {\
    from { opacity: 0; }\
    to { opacity: 1; }\
}\
@keyframes fadeInSlide {\
    from {\
        opacity: 0;\
        transform: translateY(20px);\
    }\
    to {\
        opacity: 1;\
        transform: translateY(0);\
    }\
}\
.add-button-wrapper {\
    display: flex;\
    justify-content: center;\
    width: 100%;\
    margin-bottom: 20px;\
}\
.add-button {\
    display: flex;\
    justify-content: center;\
    align-items: center;\
    width: 50px;\
    height: 50px;\
    background: linear-gradient(135deg, #6ec1e4, #3a8bd6);\
    border: none;\
    border-radius: 50%;\
    color: white;\
    font-size: 24px;\
    cursor: pointer;\
    transition: background 0.3s;\
    font-family: inherit;\
}\
.add-button:hover {\
    background: linear-gradient(135deg, #5aa9d4, #337bbf);\
}\
.plus-icon {\
    display: block;\
    width: 20px;\
    height: 20px;\
    position: relative;\
}\
.plus-icon::before,\
.plus-icon::after {\
    content: '';\
    position: absolute;\
    background-color: white;\
}\
.plus-icon::before {\
    width: 2px;\
    height: 100%;\
    left: 50%;\
    top: 0;\
    transform: translateX(-50%);\
}\
.plus-icon::after {\
    width: 100%;\
    height: 2px;\
    top: 50%;\
    left: 0;\
    transform: translateY(-50%);\
}\
.device-container {\
    display: flex;\
    flex-direction: column;\
    padding: 20px;\
    background: #fff;\
    border-radius: 15px;\
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\
    margin-bottom: 20px;\
    width: 300px;\
    opacity: 0;\
    transform: translateY(20px);\
    animation: fadeInSlide 2s forwards;\
    transition: background-color 0.3s, max-height 0.5s ease-in-out, padding 0.5s ease-in-out;\
    overflow: hidden;\
    max-height: 500px;\
}\
.device-container.disabled {\
    background-color: #f2f2f2;\
    max-height: 40px;\
    padding: 10px 20px;\
}\
.collapsed {\
    max-height: 40px;\
    overflow: hidden;\
    padding: 20px;\
}\
.collapsed input[type=\"text\"],\
.collapsed select {\
    opacity: 0;\
    pointer-events: none;\
}\
.collapsed .button {\
    opacity: 0;\
    pointer-events: none;\
}\
.disabled {\
    background: #f0f0f0;\
    color: #999;\
}\
.extra-fields {\
    max-height: 0;\
    overflow: hidden;\
    opacity: 0;\
    transition: max-height 1.5s ease, opacity 1.5s ease;\
}\
.visible {\
    max-height: 500px;\
    opacity: 1;\
}\
.error-table-container {\
    display: flex;\
    flex-direction: column;\
    align-items: center;\
    width: 700px;\
    padding: 20px;\
    background: #ffe6e6;\
    border-radius: 15px;\
    box-shadow: 0 0 10px rgba(255, 0, 0, 0.2);\
    margin: 10px 0;\
    border: 2px solid #ffcccc;\
    transition: all 0.5s ease;\
}\
\
.error-table {\
    width: 100%;\
    border-collapse: collapse;\
}\
\
.error-table thead {\
    background: #ff9999;\
    font-weight: bold;\
    color: #990000;\
}\
\
.error-table th,\
.error-table td {\
    padding: 10px;\
    text-align: center;\
    border: 1px solid #ffcccc;\
}\
\
.error-table tbody tr {\
    background: #ffcccc;\
}\
\
.error-table tbody tr:nth-child(even) {\
    background: #ffb3b3;\
}\
\
.error-header {\
    font-size: 18px;\
    font-weight: bold;\
    color: #b30000;\
    margin-bottom: 15px;\
    text-align: center;\
}\
.error-button {\
    padding: 10px 20px;\
    margin-top: 15px;\
    background: linear-gradient(135deg, #ff6666, #ff3333);\
    border: none;\
    border-radius: 25px;\
    color: white;\
    text-align: center;\
    text-decoration: none;\
    font-size: 16px;\
    cursor: pointer;\
    transition: background 0.3s, box-shadow 0.3s;\
    font-family: inherit;\
    box-shadow: 0 0 10px rgba(255, 0, 0, 0.2);\
}\
\
.error-button:hover {\
    background: linear-gradient(135deg, #ff4d4d, #e60000);\
    box-shadow: 0 0 15px rgba(255, 0, 0, 0.4);\
}\
.popup-overlay {\
    position: fixed;\
    top: 0;\
    left: 0;\
    width: 100%;\
    height: 100%;\
    background: rgba(0, 0, 0, 0.6);\
    display: none;\
    justify-content: center;\
    align-items: center;\
    z-index: 1000;\
    transition: opacity 0.3s ease;\
}\
\
.popup-content {\
    background: #ffffff;\
    width: 400px;\
    padding: 20px;\
    border-radius: 10px;\
    box-shadow: 0 0 20px rgba(0, 0, 0, 0.2);\
    text-align: center;\
    position: relative;\
    transform: scale(0.8);\
    transition: transform 0.3s ease;\
}\
\
.popup-content.show {\
    transform: scale(1);\
}\
\
.popup-header {\
    font-size: 20px;\
    margin-bottom: 15px;\
    color: #333;\
}\
\
.popup-message {\
    font-size: 16px;\
    color: #666;\
    margin-bottom: 25px;\
}\
\
.popup-button {\
    padding: 10px 20px;\
    background: linear-gradient(135deg, #6ec1e4, #3a8bd6);\
    border: none;\
    border-radius: 25px;\
    color: white;\
    text-align: center;\
    text-decoration: none;\
    font-size: 16px;\
    cursor: pointer;\
    transition: background 0.3s;\
    font-family: inherit;\
    display: inline-block;\
}\
\
.popup-button:hover {\
    background: linear-gradient(135deg, #5aa9d4, #337bbf);\
}\
\
.popup-close {\
    position: absolute;\
    top: 10px;\
    right: 10px;\
    width: 25px;\
    height: 25px;\
    background: #ff6666;\
    color: white;\
    font-size: 18px;\
    line-height: 25px;\
    text-align: center;\
    border-radius: 50%;\
    cursor: pointer;\
    transition: background 0.3s;\
}\
\
.popup-close:hover {\
    background: #ff4d4d;\
}\
.popup-password-input {\
    width: 80%;\
    padding: 10px;\
    margin-bottom: 20px;\
    border: 1px solid #ddd;\
    border-radius: 5px;\
    font-size: 16px;\
}\
\
.visible-popup {\
    display: flex;\
    opacity: 1;\
}\
\
.hidden-popup {\
    display: none;\
    opacity: 0;\
}\
.isEnabledWidget {\
    display: inline-block;\
    position: relative;\
}\
\
.status-light {\
    width: 15px;\
    height: 15px;\
    border-radius: 50%;\
    position: absolute;\
    top: 10px;\
    right: 10px;\
    box-shadow: 0 0 5px rgba(0, 0, 0, 0.5);\
    transition: all 0.3s ease;\
}\
.access-level-hidder {\
    visibility: hidden;\
    display: none;\
}\
\
.status-light.on {\
    background: radial-gradient(circle, #00ff00 20%, #006600 70%);\
    box-shadow: 0 0 10px 4px rgba(0, 255, 0, 0.6), 0 0 20px 8px rgba(0, 255, 0, 0.4);\
}\
\
.status-light.off {\
    background: radial-gradient(circle, #666 20%, #333 70%);\
    box-shadow: 0 0 5px 1px rgba(0, 0, 0, 0.4);\
}\
.room-container {\
width: 340px;\
margin-bottom: 10px;\
background: linear-gradient(135deg, #ddeff7, #c3dcec);\
border: 1px solid #b0c8d8;\
border-radius: 10px;\
box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);\
padding: 15px;\
transition: transform 0.2s ease;\
}\
\
.room-container:hover {\
transform: translateY(-2px);\
box-shadow: 0 8px 15px rgba(0, 0, 0, 0.15);\
}\
\
.room-header {\
font-size: 18px;\
font-weight: bold;\
color: #2b3a47;\
margin-bottom: 10px;\
text-transform: uppercase;\
letter-spacing: 1px;\
border-bottom: 2px solid #7aaedb;\
padding-bottom: 5px;\
}\
.color-picker {\
    display: flex;\
    flex-direction: column;\
    align-items: center;\
}\
\
.color-input {\
    width: 70px;\
    height:40px;\
    padding: 0px 0px;\
    border: none;\
    background-color: 0xffffff\
    border-radius: 4px;\
    color: white;\
    text-align: center;\
    text-decoration: none;\
    font-size: 16px;\
    cursor: pointer;\
    font-family: inherit;\
}\
\
.color-display {\
    width: 250px;\
    height: 8px;\
    border-radius: 8px;\
    background-color:0xffffff;\
    margin-bottom: 10px;\
    border: 1px solid #ccc;\
    box-shadow: 0 0 5px rgba(0, 0, 0, 0.5);\
    transition: all 0.3s ease;\
    overflow: auto;\
}\
.color-display.on {\
    box-shadow: 0 0 10px 4px rgba(0, 255, 0, 0.6), 0 0 20px 8px rgba(0, 255, 0, 0.5);\
}\
\
.color-display.off {\
    box-shadow: 0 0 5px 1px rgba(0, 0, 0, 0.3);\
}\
\
.send-color-button {\
    padding: 10px 20px;\
    font-size: 16px;\
    color: #fff;\
    background-color: #007bff;\
    border: none;\
    border-radius: 5px;\
    cursor: pointer;\
    transition: background-color 0.3s ease;\
}\
\
.send-color-button:hover {\
    background-color: #0056b3;\
}\
.led-strip {\
    display: flex;\
    padding: 10px 0px;\
    justify-content: space-between;\
    align-items: center;\
    overflow: hidden;\
}\
\
.led {\
    height: 12px;\
    border-radius: 0px; \
    margin: 0;\
}\
.led.on {\
    box-shadow: 0 0 1px 1px rgba(217, 255, 27, 0.4), 0 0 2px 1px rgba(251, 255, 0, 0.5);\
}\
\
.led.off {\
    background-color: rgba(0, 0, 0, 0.1);\
}\
.color-picker-popup {\
    position: fixed;\
    top: 10%;\
    left: 50%;\
    transform: translate(-50%, -10%);\
    background: #ffffff;\
    border: 1px solid #b0c8d8;\
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.25);\
    padding: 20px;\
    border-radius: 8px;\
    display: none;\
    flex-direction: column;\
    align-items: center;\
    z-index: 10;\
}\
.popup-backdrop {\
    position: fixed;\
    top: 0;\
    left: 0;\
    font-size: 16px;\
    width: 100%;\
    height: 100%;\
    background-color: rgba(0, 0, 0, 0.5);\
    z-index: 9;\
    display: none;\
}\
.popup-buttons {\
    display: flex;\
    gap: 10px;\
}\
.popup-buttons button {\
    padding: 5px 10px;\
    border: none;\
    font-size: 16px;\
    border-radius: 4px;\
    background: linear-gradient(135deg, #6ec1e4, #3a8bd6);\
    color: #ffffff;\
    cursor: pointer;\
}\
.popup-buttons button:hover {\
    background: linear-gradient(135deg, #5aa9d4, #337bbf);\
}\
</style>\
";

#endif