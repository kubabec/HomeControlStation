#ifndef STYLE_H
#define STYLE_H

/**
 * @file include/os/app/http/style.h
 * @brief Embedded web UI asset used by the Home Control Station HTTP interface.
 */



const char* style_css = " \
<style>\
.theme-urban {\
      --bg-card:      #2A2E35;\
      --underline:    #5A8DEE;\
      --text:         #ECEFF4;\
      --knob-bg:      #3C4148;\
      --knob-shadow:  0 4px 12px rgba(0,0,0,0.6);\
    }\
    .theme-urban {\
      background: var(--bg-card);\
      color: var(--text);\
    }\
    .theme-urban .underline {\
      background: var(--underline);\
    }\
    .theme-urban .knob {\
      background: var(--knob-bg);\
      box-shadow: var(--knob-shadow);\
    }\
body {\
display: flex;\
justify-content: center;\
align-items: flex-start;\
height: 100vh;\
margin: 0;\
background: #12151A;\
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
background: #3C4148;\
border-radius: 15px;\
box-shadow: 0 4px 12px rgba(0,0,0,0.6);\
margin-bottom: 20px;\
width: 300px;\
opacity: 1;\
transform: translateY(20px);\
}\
.header {\
font-size: 16px;\
margin-top: -7px;\
margin-bottom: 15px;\
text-align: center;\
color: #ECEFF4;\
font-family: 'Inter', sans-serif;\
font-weight: 300;\
letter-spacing: 1.0px;\
text-shadow: 0 4px 12px rgba(0, 0, 0, 0.5);\
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
color: #5A8DEE;\
text-shadow: 0 2px 9px rgba(0, 0, 0, 0.5);\
}\
.current-time{\
color: #5A8DEE;\
text-shadow: 0 2px 9px rgba(0, 0, 0, 0.5);\
}\
input[type=\"text\"], select {\
padding: 10px;\
margin-top: 5px;\
border-radius: 25px;\
border:1px;\
font-size: 14px;\
transition: background 0.3s;\
font-family: inherit;\
width: 200px;\
color: white;\
background: #2A2E35;\
}\
input[type=\"text\"]:focus, select:focus {\
background:rgb(53, 59, 68);\
outline: none;\
}\
.button-link, .button {\
flex: 1;\
margin-top: 8px;\
margin-bottom: 7px;\
padding: 10px 20px;\
background: #5A8DEE;\
border: none;\
border-radius: 25px;\
color: white;\
text-align: center;\
text-decoration: none;\
font-size: 16px;\
cursor: pointer;\
font-family: inherit;\
box-shadow:inset 0 0 0 1px #3C4148, 0 8px 24px rgba(0, 0, 0, 0.3);\
}\
.button-container {\
            display: flex;\
            flex-wrap: wrap;\
            justify-content: center;\
            gap: 10px;\
            width:100%;\
        }\
.button-link:hover, .button:hover {\
background: linear-gradient(135deg, #5aa9d4, #337bbf);\
}\
input[type=\"range\"] {\
-webkit-appearance: none;\
width: 100%;\
height: 3px;\
margin-top:20px;\
border-radius: 25px;\
background: #5A8DEE;\
outline: none;\
transition: background 0.3s;\
}\
input[type=\"range\"]::-webkit-slider-thumb {\
-webkit-appearance: none;\
appearance: none;\
width: 25px;\
height: 25px;\
border-radius: 50%;\
background:radial-gradient(circle at 30% 30%, #eee, #888);\
box-shadow:0 3px 12px rgba(0,0,0,0.7);\
cursor: pointer;\
transition: background 0.3s;\
}\
input[type=\"range\"]::-moz-range-thumb {\
width: 25px;\
height: 25px;\
border-radius: 50%;\
background: radial-gradient(circle at 30% 30%, #eee, #888);\
box-shadow:0 4px 12px rgba(0,0,0,0.8);\
cursor: pointer;\
transition: background 0.3s;\
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
.device-container {\
display: flex;\
flex-direction: column;\
padding: 20px;\
background: #3C4148;\
border-radius: 15px;\
box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\
margin-bottom: 20px;\
width: 300px;\
opacity: 0;\
transform: translateY(20px);\
animation: fadeInSlide 2s forwards;\
transition: background-color 0.3s, max-height 0.5s ease-in-out, padding 0.5s ease-in-out;\
overflow: hidden;\
max-height: 1100px;\
}\
.device-container.disabled {\
background-color: #1d2530;\
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
max-height: 900px;\
opacity: 1;\
}\
hr.custom-hr {\
  border: none;\
  height: 1px;\
  background-color:rgb(109, 165, 232);\
  margin: 1px 0;\
}\
.error-button {\
padding: 10px 20px;\
margin-top: 10px;\
margin-bottom: 5px;\
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
inset: 0;\
box-sizing: border-box;\
padding: 12px;\
background: rgba(7, 10, 15, 0.78);\
backdrop-filter: blur(4px);\
display: none;\
justify-content: center;\
align-items: center;\
z-index: 15;\
transition: opacity 0.3s ease;\
}\
\
.popup-content {\
display:grid;\
grid-template-rows:auto minmax(0,1fr);\
box-sizing:border-box;\
background: #252B34;\
width:min(440px,100%);\
max-height:min(680px,calc(100dvh - 24px));\
padding: 18px;\
border:1px solid #46515F;\
border-radius: 8px;\
box-shadow: 0 18px 48px rgba(0, 0, 0, 0.5),inset 0 1px 0 rgba(255,255,255,.04);\
text-align: center;\
position: relative;\
transform: translateY(8px) scale(.98);\
transition: transform 0.3s ease;\
-webkit-user-select: none;\
-moz-user-select: none;\
-ms-user-select: none;\
user-select: none;\
}\
\
.popup-content.show {\
transform: translateY(0) scale(1);\
}\
\
.popup-header {\
min-width:0;\
padding:0 38px 13px;\
border-bottom:1px solid #46515F;\
font-size: 18px;\
font-weight:600;\
color: #75A7FF;\
overflow:hidden;\
text-overflow:ellipsis;\
white-space:nowrap;\
}\
\
.popup-message {\
min-width:0;\
padding-top:14px;\
font-size: 15px;\
line-height:1.45;\
color: #C5CEDA;\
overflow:auto;\
overscroll-behavior:contain;\
}\
\
.popup-button {\
padding: 10px 20px;\
top: 9px;\
right: 9px;\
width: 34px;\
height: 34px;\
background: #3A424D;\
text-decoration: none;\
font-size: 22px;\
line-height: 32px;\
border:1px solid #515E6E;\
border-radius: 6px;\
font-family: inherit;\
display: inline-block;\
}\
\
.popup-button:hover {\
background: #A8444B;\
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
border-radius: 5px;\
font-size: 16px;\
color: #5A8DEE;\
background: #2A2E35;\
box-shadow:0 4px 12px rgba(0,0,0,0.3);\
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
background: #2A2E35;\
border-radius: 10px;\
box-shadow:inset 0 0 0 1px #3C4148, 0 8px 24px rgba(0, 0, 0, 0.3);\
padding: 15px;\
transition: transform 0.2s ease;\
}\
\
.room-container:hover {\
box-shadow:inset 0 0 0 1px #34393fff, 0 8px 24px rgba(0, 0, 0, 0.7);\
}\
\
.room-header {\
font-size: 18px;\
font-weight: bold;\
color: #97b0e0;\
text-transform: uppercase;\
letter-spacing: 1px;\
border-bottom:2px solid #5A8DEE;\
text-shadow:0 4px 12px rgba(0,0,0,0.5);\
padding-bottom: 5px;\
}\
.color-picker {\
display: flex;\
flex-direction: column;\
align-items: center;\
margin-top:5px;\
}\
\
.color-input {\
width: 50%;\
height:40px;\
padding: 0px 0px;\
border: none;\
background-color: #2A2E35;\
text-align: center;\
text-decoration: none;\
font-size: 16px;\
cursor: pointer;\
font-family: inherit;\
margin-bottom: 5px;\
border-radius:50px;\
}\
\
.color-display {\
width: 250px;\
height: 8px;\
border-radius: 8px;\
background-color:0xffffff;\
margin-bottom: 5px;\
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
justify-content: center;\
align-items: center;\
overflow: hidden;\
min-width:300px;\
}\
\
.led {\
height: 20px;\
width: 100%;\
background-color:rgb(21, 177, 244);\
}\
.led.on {\
//box-shadow: 0 0 1px 1px rgba(217, 255, 27, 0.4), 0 0 1px 1px rgba(251, 255, 0, 0.5);\
}\
\
.led.off {\
background-color: rgba(0, 0, 0, 0.1);\
}\
\
.ledContainer {\
padding-top: 5px;\
padding-bottom: 5px;\
width:100%;\
background-color:#2A2E35;\
}\
.ledContainer.marked {\
background-color:rgba(93, 190, 255, 0.9);\
}\
.color-picker-popup {\
position: fixed;\
top: 25%;\
background: #2A2E35;\
color: #5A8DEE;\
box-shadow: 0 4px 10px rgba(0, 0, 0, 0.25);\
padding: 20px;\
border-radius: 8px;\
display: none;\
flex-direction: column;\
align-items: center;\
z-index: 16;\
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
.container.loading {\
background-color: rgba(255, 255, 255, 0.8);\
}\
.container .loading-overlay {\
position: absolute;\
top: 0;\
left: 0;\
width: 100%;\
height: 100%;\
background-color: #3C4148;\
display: flex;\
align-items: center;\
justify-content: center;\
z-index: 10;\
border-radius: inherit; \
pointer-events: none;\
}\
.loading-overlay .spinner {\
width: 40px;\
height: 40px;\
border: 4px solid transparent;\
border-top: 4px solid #007bff;\
border-radius: 50%;\
animation: spin 1s linear infinite;\
}\
.loading-overlay .loading-text {\
font-size: 16px;\
color: #007bff;\
margin-left: 10px;\
font-weight: bold;\
}\
@keyframes spin {\
from {\
transform: rotate(0deg);\
}\
to {\
transform: rotate(360deg);\
}\
}\
.notifications-popup {\
    position: fixed;\
    top: 50%;\
    left: 50%;\
    transform: translate(-50%, -50%);\
    background: #ffffff;\
    border-radius: 10px;\
    box-shadow: 0 4px 10px rgba(0, 0, 0, 0.3);\
    width: 80%;\
    max-width: 400px;\
    padding: 20px;\
    z-index: 11;\
    display: none;\
}\
.notification {\
    border: 1px solid;\
    border-radius: 5px;\
    padding: 10px;\
    margin-bottom: 10px;\
    display: flex;\
    flex-direction: column;\
    align: left;\
    box-shadow:0 4px 12px rgba(0,0,0,0.6);\
}\
\
.notification-title {\
    font-weight: bold;\
    margin-bottom: 5px;\
}\
\
.notification.error {\
    background-color: #3C4148;\
    border-color: #ff5c5c;\
    color: #ff8383;\
}\
\
.notification.warning {\
    background-color: #3C4148;\
    border-color: #ffc107;\
    color:rgb(239, 193, 10);\
}\
\
.notification.info {\
    background-color: #3C4148;\
    border-color: #17a2b8;\
    color:rgb(29, 199, 251);\
}\
.project-name {\
  position: fixed;\
  bottom: 10px;\
  width: 100%;\
  text-align: center;\
  font-size: 24px;\
  color: rgba(0, 0, 0, 0.1);\
  z-index: -1; \
  font-weight: bold;\
  pointer-events: none; \
  user-select: none; \
}\
.device-left {\
display: flex;\
align-items: center;\
}\
footer {\
	text-align: center;\
	font-size: 11px;\
	color:rgb(12, 119, 152);\
	padding: 8px 0;\
}\
footer a {\
	color: rgb(11, 94, 119);\
	text-decoration: none;\
	margin: 0 4px;\
}\
.switch {\
    position: absolute;\
    top:-38px;\
    right:0px;\
    width: 56px;\
    height: 32px;\
    background: #484646;\
    box-shadow:0 4px 12px rgba(0,0,0,0.3);\
    border-radius: 16px;\
    cursor: pointer;\
    transition: background 0.3s;\
}\
.switch .thumb {\
    position: absolute;\
    top: 2px;\
    left: 2px;\
    width: 28px;\
    height: 28px;\
    border-radius: 50%;\
    background: radial-gradient(circle at 30% 30%, #eee, #888);\
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);\
    transition: left 0.3s, box-shadow 0.3s;\
}\
.segThumb{\
  background:radial-gradient(circle at 30% 40%, #accefb, #12009d);\
  position: absolute;\
  top: 2px;\
  left: 2px;\
  width: 28px;\
  height: 28px;\
  border-radius: 50%;\
  box-shadow: 0 2px 4px rgba(0,0,0,0.1);\
  transition: left 0.3s, box-shadow 0.3s;\
}\
.switch.on {\
    background: #4caf50;\
}\
.switch.on .thumb {\
    left: calc(100% - 30px);\
    box-shadow: 0 2px 8px rgba(76,175,80,0.6);\
}\
.switch.on .segThumb {\
    left: calc(100% - 30px);\
    box-shadow: 0 2px 8px rgba(76,175,80,0.6);\
}\
.toggler-container {\
  position: relative;\
}\
:root {\
  --g-dark: #2a2d34;\
  --g-mid:  #3a3f4a;\
  --blue:   #5a74e2;\
  --fg:     #d0d0d0;\
}\
.table-graphite {\
  width:100%; border-collapse:collapse;\
  background:var(--g-dark); color:var(--fg);\
  font-family:sans-serif;\
  width:70%;\
  border-radius:8px; box-shadow:0 2px 8px rgba(0,0,0,.5);\
  overflow:hidden;\
}\
.table-graphite th,\
.table-graphite td {\
  text-align:center;\
  padding:10px 16px; border-bottom:1px solid var(--g-mid);\
}\
.table-graphite thead {\
  background:var(--g-mid);\
}\
.table-graphite thead th {\
  border-bottom:2px solid var(--blue);\
  text-transform:uppercase; font-size:.9em;\
}\
.table-graphite tbody tr:nth-child(even) {\
  background: #2f323b;\
}\
.table-graphite tbody tr:hover {\
  background: #363a45;\
}\
.table-graphite .badge {\
  display:inline-block; padding:2px 8px; border-radius:12px;\
  background:var(--blue); color:#fff; font-size:.75em;\
}\
.table-graphite .badge.inactive {\
  background:#555b6e;\
}\
.segLedTog{\
position: relative;\
left: 80%;\
margin-left: 10px;\
top:0%;\
}\
.segColDisp{\
  position:relative;\
  top:25px;\
  margin-left: -22%;\
}\
.memory-bar {\
position: relative;\
width: 100%;\
max-width: 400px;\
height: 20px;\
background-color: #333;\
border-radius: 5px;\
overflow: hidden;\
}\
.memory-bar-fill {\
background-color: #2196f3;\
height: 100%;\
width: 0%;\
transition: width 0.3s ease;\
}\
.sensor-error {\
color: #ff4c4c;\
background-color: #2a2a2a;\
padding: 10px;\
border-radius: 10px;\
font-weight: bold;\
margin-top: 5px;\
box-shadow: 0 0 5px rgba(255, 0, 0, 0.5);\
display: flex;\
text-align: center;\
justify-content: center;\
align-items: center;\
height: 100%; /* Możesz dostosować */\
}\
.icon-btn {\
position: relative;\
display: inline-flex;\
align-items: center;\
justify-content: center;\
margin-top:7px;\
height: 2.3rem;\
flex:1;\
background-color: #5A8DEE;\
border: none;\
border-radius: 15px;\
color: #ffffff;\
font-size: 1.8rem;\
cursor: pointer;\
box-shadow: inset 0 1px 0 rgba(255,255,255,0.05),\
            0 2px 6px rgba(0,0,0,0.4);\
}\
.icon-btn:hover {\
background:linear-gradient(135deg, #5aa9d4, #337bbf);\
}\
.icon-btn:active {\
background-color: #24303e;\
box-shadow: inset 0 2px 4px rgba(0,0,0,0.6);\
}\
\
.icon-btn::before {\
content: '';\
display: inline-block;\
}\
.icon-btn.on::before {\
content: 'ON';\
font-size:1.3rem;\
}\
.icon-btn.off::before {\
content: 'OFF';\
font-size:1.3rem;\
text-shadow: 0 0 20px rgba(0,255,0,1);\
color:#0df90d;\
}\
.icon-btn.play::before {\
content: '\\25B6';\
margin-left: 2px;\
}\
.icon-btn.stop::before {\
content: '\\25A0';\
color:#64ff5d;\
text-shadow: 0 0 20px rgba(0, 255, 0, 3);\
}\
.icon-btn.save::before {\
content: '\\1F4BE';\
}\
.icon-btn.load::before {\
content: '\\1F4E5';\
}\
.icon-btn.settings::before {\
content: '\\2699';\
}\
.icon-btn.presets::before {\
content: '\\1F5C3';\
}\
.icon-btn.ok::before {\
content: '\\2714';\
}\
.device-readouts {\
display:grid;\
grid-template-columns:repeat(2,minmax(0,1fr));\
gap:9px;\
width:100%;\
margin:2px 0 10px;\
}\
.device-readout {\
display:flex;\
flex-direction:column;\
justify-content:center;\
min-height:52px;\
padding:9px 11px;\
box-sizing:border-box;\
background:#2A2E35;\
border:1px solid rgba(90,141,238,.18);\
border-radius:12px;\
box-shadow:inset 0 1px 0 rgba(255,255,255,.025),0 3px 9px rgba(0,0,0,.18);\
}\
.device-readout:last-child:nth-child(odd) { grid-column:1 / -1; }\
.device-readout-label {\
color:#8A94A6;\
font-size:10px;\
font-weight:600;\
letter-spacing:.08em;\
line-height:1.2;\
text-transform:uppercase;\
}\
.device-readout-value {\
margin-top:4px;\
color:#ECEFF4;\
font-size:15px;\
font-weight:600;\
line-height:1.25;\
overflow-wrap:anywhere;\
}\
.device-readout-value.tone-accent { color:#86AEFF; }\
.device-readout-value.tone-success { color:#6EDC8C; }\
.device-readout-value.tone-warning { color:#F3C969; }\
.device-readout-value.tone-danger { color:#FF7B7B; }\
.advanced-controls-form {\
display:flex;\
flex-direction:column;\
gap:14px;\
padding:6px 2px 2px;\
text-align:left;\
}\
.advanced-status-card {\
display:grid;\
grid-template-columns:repeat(2,minmax(0,1fr));\
gap:8px;\
padding:12px;\
background:#20242A;\
border:1px solid rgba(90,141,238,.22);\
border-radius:12px;\
}\
.advanced-status-item { display:flex;flex-direction:column;gap:3px;min-width:0; }\
.advanced-status-item.wide { grid-column:1 / -1; }\
.advanced-status-label { color:#8A94A6;font-size:10px;font-weight:600;letter-spacing:.08em;text-transform:uppercase; }\
.advanced-status-value { color:#ECEFF4;font-size:14px;font-weight:600;overflow-wrap:anywhere; }\
.advanced-status-value.good { color:#6EDC8C; }\
.advanced-status-value.warn { color:#F3C969; }\
.advanced-status-value.danger { color:#FF7B7B; }\
/* HomeControlStation control-surface theme */\
:root {\
--surface-0:#101419;--surface-1:#191F26;--surface-2:#222A33;--surface-3:#2C3540;\
--steel:#465464;--steel-soft:#34404D;--accent:#5B91F2;--accent-strong:#76A8FF;\
--text-strong:#F2F5F8;--text:#C8D0DA;--text-muted:#8E9AAA;\
--success:#55C98A;--warning:#F2C66D;--danger:#F06A72;--shadow:0 12px 28px rgba(0,0,0,.32);\
color-scheme:dark;\
}\
* { box-sizing:border-box; }\
html { min-height:100%;background:var(--surface-0); }\
body {\
display:block;min-width:0;min-height:100dvh;height:auto;margin:0;\
padding:16px max(12px,env(safe-area-inset-right)) max(20px,env(safe-area-inset-bottom)) max(12px,env(safe-area-inset-left));\
background:linear-gradient(145deg,#171D24 0%,var(--surface-0) 48%,#0C1014 100%);\
color:var(--text);font-family:\"Avenir Next\",\"Segoe UI\",Helvetica,Arial,sans-serif;font-size:15px;line-height:1.4;letter-spacing:0;\
overflow-x:hidden;animation:fadeInBody .35s ease-out forwards;\
}\
.wrapper { width:100%;max-width:1180px;margin:0 auto;gap:14px; }\
#rooms { display:grid;grid-template-columns:repeat(auto-fit,minmax(min(100%,360px),1fr));gap:14px;width:100%;align-items:start; }\
#rooms > .room-container:only-child { width:min(100%,620px);justify-self:center; }\
.room-container {\
width:100%;min-width:0;margin:0;padding:12px;background:var(--surface-1);\
border:1px solid var(--steel-soft);border-radius:8px;box-shadow:var(--shadow);\
}\
.room-container:hover { box-shadow:var(--shadow);border-color:var(--steel); }\
.room-header {\
min-height:42px;padding:2px 70px 9px 2px;color:var(--text-strong);font-size:17px;font-weight:650;\
text-transform:none;letter-spacing:0;border-bottom:2px solid var(--accent);text-shadow:none;overflow-wrap:anywhere;\
}\
.container,.device-container {\
position:relative;width:min(100%,420px);min-width:0;margin:0 auto 12px;padding:16px;background:var(--surface-2);\
border:1px solid var(--steel);border-radius:8px;box-shadow:0 8px 18px rgba(0,0,0,.26);transform:none;\
}\
.room-container > .container { width:100%;margin-top:12px;margin-bottom:0; }\
.header { margin:0 0 14px;color:var(--text-strong);font-size:17px;font-weight:600;letter-spacing:0;text-shadow:none; }\
.header2 { color:var(--text);font-size:14px;font-weight:600; }\
label { min-width:0;gap:12px;margin:10px 0;color:var(--text);font-size:14px;text-shadow:none; }\
input,select,button { min-width:0;font:inherit;letter-spacing:0; }\
input[type=\"text\"],input[type=\"password\"],input[type=\"number\"],input[type=\"datetime-local\"],input[type=\"time\"],select {\
width:min(220px,60%);min-height:44px;margin:0;padding:9px 12px;border:1px solid var(--steel);border-radius:6px;outline:none;\
background:var(--surface-1);color:var(--text-strong);transition:border-color .16s,box-shadow .16s,background .16s;\
}\
input:focus,select:focus { background:var(--surface-1);border-color:var(--accent);box-shadow:0 0 0 3px rgba(91,145,242,.18); }\
input:disabled,input[readonly] { color:var(--text-muted);opacity:1; }\
input[type=\"checkbox\"] { width:20px;height:20px;accent-color:var(--accent);flex:0 0 auto; }\
input[type=\"range\"] { height:4px;margin:16px 2px 8px;background:var(--steel);accent-color:var(--accent); }\
input[type=\"range\"]::-webkit-slider-thumb { width:24px;height:24px;background:var(--accent-strong);border:3px solid var(--surface-2);box-shadow:0 2px 8px rgba(0,0,0,.45); }\
input[type=\"range\"]::-moz-range-thumb { width:20px;height:20px;background:var(--accent-strong);border:3px solid var(--surface-2);box-shadow:0 2px 8px rgba(0,0,0,.45); }\
.button-link,.button,.popup-button,.send-color-button,.advanced-action,.advanced-primary {\
display:inline-flex;align-items:center;justify-content:center;min-height:44px;max-width:100%;margin:6px 2px;padding:9px 16px;\
border:1px solid #566576;border-radius:6px;background:#34404D;color:var(--text-strong);\
font-size:14px;font-weight:650;line-height:1.2;text-align:center;text-decoration:none;cursor:pointer;\
box-shadow:0 5px 12px rgba(13,31,55,.28);transition:background .16s,border-color .16s,transform .16s;\
}\
.button-link:hover,.button:hover,.popup-button:hover,.send-color-button:hover,.advanced-primary:hover { background:#465464;border-color:#708196; }\
.button-link:active,.button:active,.popup-button:active { transform:translateY(1px); }\
.button-container,.controls { display:flex;flex-wrap:wrap;justify-content:center;gap:8px;width:100%;margin:8px auto; }\
.error-button {\
display:inline-flex;align-items:center;justify-content:center;min-height:44px;margin:6px 2px;padding:9px 16px;\
border:1px solid #D7787D;border-radius:6px;background:#783E45;color:#FFE9EA;font-size:14px;font-weight:650;box-shadow:none;\
}\
.error-button:hover { background:#914750;box-shadow:none; }\
hr.custom-hr { background:var(--steel);margin:14px 0; }\
.current-time { width:min(100%,420px);margin:0 auto 10px;padding:8px 12px;color:var(--text-muted);font-size:12px;text-align:center; }\
.memory-bar { height:10px;background:var(--surface-0);border:1px solid var(--steel-soft);border-radius:5px; }\
.memory-bar-fill { background:var(--accent); }\
.switch { top:-38px;right:0;width:52px;height:28px;border:1px solid var(--steel);border-radius:14px;background:var(--surface-3);box-shadow:none; }\
.switch .thumb,.segThumb { top:3px;left:3px;width:20px;height:20px;background:var(--text-muted);box-shadow:none; }\
.switch.on { background:#236947;border-color:var(--success); }\
.switch.on .thumb,.switch.on .segThumb { left:calc(100% - 23px);background:var(--success);box-shadow:none; }\
.status-light { width:11px;height:11px;top:12px;right:12px;border:2px solid var(--surface-2); }\
.status-light.on { background:var(--success);box-shadow:0 0 0 3px rgba(85,201,138,.14); }\
.status-light.off { background:#687382;box-shadow:none; }\
.icon-btn { min-height:44px;height:44px;margin-top:6px;border:1px solid #566576;border-radius:6px;background:#34404D;color:var(--text-strong);box-shadow:none; }\
.icon-btn:hover { background:#465464;border-color:#708196; }\
.icon-btn.device-state { gap:9px;font-size:14px;font-weight:700; }\
.icon-btn.device-state::before { width:9px;height:9px;border-radius:50%;background:currentColor;box-shadow:none; }\
.icon-btn.state-enabled { background:#236947;border-color:var(--success);color:#F3FFF8; }\
.icon-btn.state-enabled:hover { background:#2B7A55;border-color:#78DDA6; }\
.icon-btn.state-enabled::before { box-shadow:0 0 0 4px rgba(243,255,248,.12); }\
.icon-btn.state-disabled { background:var(--surface-3);border-color:var(--steel);color:#B6C0CD; }\
.icon-btn.state-disabled:hover { background:#37424F;border-color:#617184;color:var(--text-strong); }\
.icon-btn.off::before { color:#D8E5FA;text-shadow:none; }\
.icon-btn.stop::before { color:#FFE1E3;text-shadow:none; }\
.device-readout,.advanced-status-card,.advanced-switch { background:var(--surface-1);border-color:var(--steel-soft);border-radius:6px;box-shadow:none; }\
.device-readout-label,.advanced-status-label { color:var(--text-muted);letter-spacing:.04em; }\
.device-readout-value,.advanced-status-value { color:var(--text-strong); }\
.advanced-field > span { color:var(--text); }\
.advanced-field select,.advanced-field input[type=\"number\"] { width:100%; }\
.advanced-action { background:var(--surface-3);border-color:var(--steel);color:var(--text-strong); }\
.advanced-action.primary,.advanced-primary { background:#3A4653;border-color:#667587;color:var(--text-strong); }\
.advanced-action.danger { background:#783E45;border-color:#D7787D;color:#FFE9EA; }\
.color-input { width:64px;height:44px;padding:3px;border:1px solid var(--steel);border-radius:6px;background:var(--surface-1); }\
.color-display { width:100%;border-radius:4px; }\
.led-strip { width:100%;min-width:0;padding:10px 0;overflow-x:auto;justify-content:flex-start; }\
.ledContainer { min-width:9px;background:var(--surface-1); }\
.ledContainer.marked { background:var(--accent); }\
.container .loading-overlay { inset:0;width:auto;height:auto;max-width:100%;max-height:100%;padding:12px;background:rgba(34,42,51,.94);overflow:hidden; }\
.loading-overlay .spinner { flex:0 0 auto;width:34px;height:34px;border-width:3px;border-top-color:var(--accent); }\
.loading-overlay .loading-text { min-width:0;margin-left:10px;color:var(--text-strong);overflow-wrap:anywhere; }\
.notifications-popup { background:var(--surface-2);color:var(--text);border:1px solid var(--steel); }\
.notification { border-radius:6px;background:var(--surface-1)!important;box-shadow:none; }\
.table-graphite {\
display:block;width:100%;max-width:1180px;margin:4px auto 10px;overflow-x:auto;overscroll-behavior-inline:contain;\
background:var(--surface-1);border:1px solid var(--steel-soft);border-radius:8px;box-shadow:var(--shadow);font-family:inherit;\
}\
.table-graphite thead,.table-graphite tbody { display:table;width:max-content;min-width:100%;table-layout:auto; }\
.table-graphite th,.table-graphite td { min-width:140px;padding:12px;border-bottom:1px solid var(--steel-soft);text-align:left;vertical-align:middle; }\
.table-graphite th:first-child,.table-graphite td:first-child { min-width:96px; }\
.table-graphite thead { background:var(--surface-3); }\
.table-graphite thead th { border-bottom:2px solid var(--accent);color:var(--text-muted);font-size:11px;letter-spacing:.04em;white-space:normal; }\
.table-graphite tbody tr:nth-child(even) { background:#1D242C; }\
.table-graphite tbody tr:hover { background:#252E38; }\
.table-graphite input,.table-graphite select { width:100%;min-width:120px; }\
.table-graphite .button,.table-graphite .error-button { width:100%;margin:0; }\
.cell-conditions { display:grid;gap:6px; }\
.rtc-interval label,.rtc-days label { margin:6px 0; }\
.rtc-days { display:flex;flex-wrap:wrap;gap:5px; }\
.rtc-days label { justify-content:flex-start;gap:4px;padding:5px 7px;background:var(--surface-2);border:1px solid var(--steel-soft);border-radius:4px; }\
.sensor-error { min-height:64px;height:auto;background:var(--surface-1);border:1px solid #8F434A;border-radius:6px;box-shadow:none; }\
footer { color:#647283;padding:18px 0 6px; }\
footer a { color:#8195AD; }\
body input[type=\"number\"] { width:min(220px,60%);min-height:44px;margin:0;padding:9px 12px;border:1px solid var(--steel);border-radius:6px;background:var(--surface-1);color:var(--text-strong); }\
body input[type=\"number\"]:focus { border-color:var(--accent);box-shadow:0 0 0 3px rgba(91,145,242,.18); }\
body .advanced-field > span { color:var(--text); }\
body .advanced-switch { min-height:44px;background:var(--surface-1);border:1px solid var(--steel-soft);border-radius:6px;color:var(--text); }\
body .advanced-switch input[type=\"checkbox\"] { accent-color:var(--accent); }\
body .advanced-action,body .advanced-primary { min-height:44px;border:1px solid var(--steel);border-radius:6px;background:var(--surface-3);color:var(--text-strong);box-shadow:none; }\
body .advanced-action.primary,body .advanced-primary { background:#3A4653;border-color:#667587;color:var(--text-strong); }\
body .advanced-action.danger { background:#783E45;border-color:#D7787D;color:#FFE9EA; }\
#popup-content { display:flex;flex-direction:column;align-items:stretch;gap:16px; }\
#popup-content .popup-header { margin:0;color:var(--text-strong); }\
#popup-content .popup-message { padding:4px 0;overflow:visible;text-align:center; }\
#popup-content .popup-button { align-self:center;width:min(100%,220px);height:auto;margin:0;padding:11px 18px;font-size:14px;line-height:1.2; }\
#password-popup-content { display:flex;flex-direction:column;align-items:stretch;gap:14px; }\
#password-popup-content .popup-header { margin:0; }\
#password-popup-content .popup-message { padding:0;overflow:visible;text-align:center; }\
#password-popup-content .popup-password-input { width:100%;min-height:46px;margin:0;color:var(--text-strong); }\
#password-popup-content .popup-button { align-self:stretch;width:100%;height:auto;margin:0;line-height:1.2; }\
.container > form { display:flex;flex-direction:column;align-items:stretch;gap:8px; }\
.container > form > .header,.container > form > label,.container > form > hr,.container > form > br,.container > form > .access-level-hidder { grid-column:1 / -1; }\
.container > form > .access-level-hidder label { width:100%; }\
.container > form > .button-link { align-self:center;width:min(100%,300px);margin:0; }\
.container > form > .error-button { align-self:flex-start;margin:0; }\
.settings-section-title { margin:4px 0 2px;color:var(--text-muted);font-size:11px;font-weight:700;text-align:center;text-transform:uppercase; }\
.maintenance-button { display:inline-flex;align-items:center;justify-content:center;align-self:center;width:min(100%,300px);min-height:44px;padding:9px 16px;border:1px solid #667587;border-radius:6px;background:#3A4653;color:var(--text-strong);font-weight:650;cursor:pointer; }\
.maintenance-button:hover { background:#465464;border-color:#7A8B9F; }\
.maintenance-button.danger { border-color:#9E555D;background:#713E45;color:#FFE9EA; }\
.maintenance-button.danger:hover { background:#874951; }\
.device-container .slot-header { flex:0 0 auto;width:100%;min-height:36px;margin:0;gap:10px;color:var(--text);white-space:nowrap; }\
.device-container .slot-header .status-text { margin-left:auto;color:var(--text-muted); }\
.device-container.disabled { min-height:58px;max-height:58px;padding:10px 16px; }\
.device-container.disabled .slot-header { min-height:36px; }\
@media (max-width:520px) {\
body { padding-top:8px;font-size:14px; }\
body .room-container { padding:8px; }\
body .container,body .device-container { width:100%;padding:14px; }\
body .device-container label { gap:8px; }\
body .device-container input[type=\"number\"],body .device-container select,body .device-container input[type=\"text\"] { width:min(190px,58%); }\
body .table-graphite { border-radius:6px; }\
body .table-graphite th,body .table-graphite td { min-width:128px;padding:9px; }\
body .wrapper > .button,body .wrapper > .button-link { width:min(100%,420px);flex:none; }\
.container > form > .button-link,.container > form > .error-button,.maintenance-button { width:100%;align-self:stretch; }\
}\
.advanced-field {\
display:grid;\
grid-template-columns:minmax(0,1fr) minmax(150px,1.25fr);\
gap:14px;\
align-items:center;\
margin:0;\
}\
.advanced-field > span { color:#95BAFF;font-size:14px; }\
.advanced-field select,.advanced-field input[type=\"number\"] { width:100%;margin:0;box-sizing:border-box; }\
.advanced-switch {\
display:flex;\
align-items:center;\
justify-content:space-between;\
min-height:42px;\
margin:0;\
padding:0 12px;\
background:#343942;\
border-radius:11px;\
color:#DCE5F5;\
}\
.advanced-switch input[type=\"checkbox\"] { width:19px;height:19px;accent-color:#5A8DEE;order:2; }\
.advanced-actions { display:grid;grid-template-columns:repeat(3,1fr);gap:9px;margin-top:2px; }\
.advanced-action,.advanced-primary {\
min-height:42px;\
padding:9px 12px;\
border:0;\
border-radius:12px;\
background:#3C4148;\
color:#ECEFF4;\
font-family:inherit;\
font-size:14px;\
font-weight:600;\
cursor:pointer;\
box-shadow:inset 0 0 0 1px rgba(255,255,255,.06),0 4px 10px rgba(0,0,0,.2);\
}\
.advanced-action:hover { background:#484F5A; }\
.advanced-action.primary,.advanced-primary { background:#5A8DEE;color:white; }\
.advanced-action.danger { background:#61373B;color:#FFD9D9; }\
.advanced-primary { width:100%;margin-top:2px; }\
input[type=\"number\"] {\
width:200px;\
padding:10px 12px;\
margin-top:5px;\
border:1px solid rgba(90,141,238,.25);\
border-radius:18px;\
box-sizing:border-box;\
font-family:inherit;\
font-size:14px;\
color:#ECEFF4;\
background:#2A2E35;\
}\
input[type=\"number\"]:focus { border-color:#5A8DEE;outline:none;box-shadow:0 0 0 2px rgba(90,141,238,.15); }\
@media (max-width:520px) {\
.popup-overlay { padding:8px;align-items:center; }\
.popup-content { width:100%;max-height:calc(100dvh - 16px);padding:14px; }\
.popup-header { padding-bottom:11px;font-size:17px; }\
.popup-message { padding-top:12px; }\
.advanced-field { grid-template-columns:1fr;gap:5px; }\
.advanced-actions { grid-template-columns:1fr; }\
.device-container { width:calc(100vw - 56px); }\
.device-container label { gap:10px; }\
.device-container input[type=\"number\"],.device-container select,.device-container input[type=\"text\"] { width:min(200px,55vw); }\
}\
</style>\
";

#endif