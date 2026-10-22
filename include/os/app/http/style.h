#ifndef STYLE_H
#define STYLE_H

/**
 * @file include/os/app/http/style.h
 * @brief Embedded web UI asset used by the Home Control Station HTTP interface.
 *
 * Single "liquid glass" stylesheet: graphite background, translucent glass panels and a blue accent
 * taken from the HomeControlStation logo. Every selector used by the dashboard, settings pages,
 * popups and device widgets is defined here; only presentation lives in this file.
 */



const char* style_css = " \
<style>\
:root {\
--bg-0:#090E16;--bg-1:#0E1621;--bg-2:#111D2B;\
--glass:linear-gradient(135deg,rgba(182,215,255,.14) 0%,rgba(119,153,195,.075) 48%,rgba(179,210,237,.04) 100%);\
--glass-card:linear-gradient(135deg,rgba(182,215,255,.09) 0%,rgba(119,153,195,.045) 100%);\
--glass-well:rgba(8,14,22,.42);\
--glass-hover:rgba(182,215,255,.18);\
--line:rgba(168,196,226,.19);--line-soft:rgba(169,191,217,.13);--line-strong:rgba(168,196,226,.34);\
--blue-1:#5CB2FF;--blue-2:#3B7FF0;--blue-light:#8DCBFF;\
--blue-grad:linear-gradient(135deg,#5CB2FF 0%,#3B7FF0 100%);\
--text:#F2F7FF;--text-soft:#C9D6E6;--muted:#9DACBF;\
--success:#72D8B1;--warning:#F2C66D;--danger:#FF8A8F;--coral:#FF9B86;--teal:#7FE0D8;\
--r-xl:22px;--r-l:17px;--r-m:13px;--r-s:10px;\
--shadow:0 14px 40px rgba(0,0,0,.35),inset 0 1px 0 rgba(255,255,255,.08);\
color-scheme:dark;\
}\
* { box-sizing:border-box; }\
html { min-height:100%;background:var(--bg-0); }\
body {\
display:block;min-width:0;min-height:100dvh;height:auto;margin:0;\
padding:18px max(12px,env(safe-area-inset-right)) max(20px,env(safe-area-inset-bottom)) max(12px,env(safe-area-inset-left));\
background:radial-gradient(60vmax 60vmax at 88% -8%,rgba(76,157,255,.20),transparent 65%),radial-gradient(50vmax 50vmax at -12% 72%,rgba(76,157,255,.09),transparent 65%),linear-gradient(145deg,#0A111A 0%,var(--bg-2) 52%,var(--bg-0) 100%);\
background-attachment:fixed;color:var(--text);\
font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,'Helvetica Neue',Arial,sans-serif;font-size:15px;line-height:1.4;letter-spacing:0;\
overflow-x:hidden;opacity:0;animation:fadeInBody .35s ease-out forwards;\
-webkit-tap-highlight-color:transparent;\
}\
@keyframes fadeInBody { from { opacity:0; } to { opacity:1; } }\
@keyframes fadeInSlide { from { opacity:0;transform:translateY(12px); } to { opacity:1;transform:translateY(0); } }\
@keyframes spin { from { transform:rotate(0deg); } to { transform:rotate(360deg); } }\
@media (prefers-reduced-motion:reduce) { * { animation-duration:.01ms!important;transition-duration:.01ms!important; } }\
.wrapper { display:flex;flex-direction:column;align-items:center;width:100%;max-width:1180px;margin:0 auto;gap:14px; }\
.connection-lost .wrapper > *:not(.popup-overlay) { filter:blur(5px);pointer-events:none; }\
.connection-lost::after {\
content:'Connection with the server was lost';position:fixed;z-index:1000;top:50%;left:50%;width:min(90vw,360px);padding:16px 18px;\
border:1px solid rgba(242,198,109,.55);border-radius:var(--r-l);background:rgba(14,22,33,.9);\
-webkit-backdrop-filter:blur(16px);backdrop-filter:blur(16px);\
color:#FFD98A;font-size:16px;font-weight:700;text-align:center;box-shadow:0 18px 44px rgba(0,0,0,.5);transform:translate(-50%,-50%);pointer-events:none;\
}\
#rooms { display:grid;grid-template-columns:repeat(auto-fit,minmax(min(100%,360px),1fr));gap:16px;width:100%;align-items:start; }\
#rooms > .room-container:only-child { width:min(100%,620px);justify-self:center; }\
.room-container {\
position:relative;width:100%;min-width:0;margin:0;padding:16px 14px 14px;\
background:var(--glass);border:1px solid var(--line);border-radius:var(--r-xl);box-shadow:var(--shadow);\
-webkit-backdrop-filter:blur(18px) saturate(140%);backdrop-filter:blur(18px) saturate(140%);\
}\
.room-container::before {\
content:'';position:absolute;top:0;left:22px;right:22px;height:3px;border-radius:0 0 3px 3px;\
background:var(--blue-grad);opacity:.9;pointer-events:none;\
}\
.room-header {\
min-height:44px;padding:4px 66px 12px 4px;border-bottom:1px solid var(--line-soft);\
color:var(--text);font-size:20px;font-weight:700;text-transform:none;letter-spacing:0;overflow-wrap:anywhere;\
}\
.toggler-container { position:relative; }\
.container,.device-container {\
position:relative;width:min(100%,420px);min-width:0;margin:0 auto 14px;padding:16px;\
background:var(--glass-card);border:1px solid var(--line-soft);border-radius:var(--r-l);\
box-shadow:inset 0 1px 0 rgba(255,255,255,.06),0 8px 20px rgba(0,0,0,.2);\
}\
.room-container > .container { width:100%;margin-top:12px;margin-bottom:0; }\
.room-container > .container .header { padding:0 24px 0 0;text-align:left; }\
.header { margin:0 0 14px;color:var(--text);font-size:17px;font-weight:650;text-align:center;letter-spacing:0; }\
.header2 { margin-bottom:5px;color:var(--text-soft);font-size:14px;font-weight:600;text-align:center; }\
.current-time { width:min(100%,420px);margin:0 auto 10px;padding:8px 12px;color:var(--muted);font-size:12px;text-align:center; }\
hr.custom-hr { height:1px;margin:14px 0;border:none;background:var(--line-soft); }\
.status-light { position:absolute;top:16px;right:16px;width:10px;height:10px;border-radius:50%;transition:background .25s,box-shadow .25s; }\
.status-light.on { background:var(--success);box-shadow:0 0 0 4px rgba(114,216,177,.16),0 0 12px rgba(114,216,177,.55); }\
.status-light.off { background:#6D7B8D;box-shadow:0 0 0 4px rgba(109,123,141,.12); }\
.switch {\
position:absolute;top:-42px;right:0;width:52px;height:30px;border:1px solid var(--line);border-radius:15px;\
background:rgba(8,14,22,.5);cursor:pointer;transition:background .25s,border-color .25s;\
}\
.switch .thumb { position:absolute;top:3px;left:3px;width:22px;height:22px;border-radius:50%;background:#93A4B7;box-shadow:0 2px 6px rgba(0,0,0,.4);transition:left .25s,background .25s; }\
.switch.on { background:rgba(85,198,154,.24);border-color:rgba(103,213,169,.75); }\
.switch.on .thumb { left:calc(100% - 25px);background:#6BDFAD;box-shadow:0 0 10px rgba(107,223,173,.5); }\
label {\
display:flex;align-items:center;justify-content:space-between;gap:12px;min-width:0;margin:10px 0;\
color:var(--text-soft);font-size:14px;\
}\
input,select,button { min-width:0;font:inherit;letter-spacing:0; }\
input[type='text'],input[type='password'],input[type='number'],input[type='datetime-local'],input[type='time'],select {\
width:min(220px,60%);min-height:44px;margin:0;padding:9px 14px;border:1px solid var(--line);border-radius:var(--r-m);outline:none;\
background:var(--glass-well);color:var(--text);font-size:14px;\
transition:border-color .16s,box-shadow .16s,background .16s;\
}\
input:focus,select:focus { border-color:rgba(111,186,255,.8);box-shadow:0 0 0 3px rgba(92,178,255,.2); }\
input:disabled,input[readonly] { color:var(--muted);opacity:1; }\
select {\
padding-right:34px;-webkit-appearance:none;appearance:none;\
background-image:url(\"data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='12' height='8' viewBox='0 0 12 8'%3E%3Cpath d='M1 1.5l5 5 5-5' fill='none' stroke='%239DACBF' stroke-width='1.8' stroke-linecap='round' stroke-linejoin='round'/%3E%3C/svg%3E\");\
background-repeat:no-repeat;background-position:right 13px center;\
}\
select option { background:#162231;color:var(--text); }\
input[type='checkbox'] { width:20px;height:20px;accent-color:var(--blue-1);flex:0 0 auto; }\
input[type='color'] { cursor:pointer; }\
input[type='range'] {\
-webkit-appearance:none;appearance:none;display:block;width:100%;height:8px;margin:18px 0 8px;padding:0;border:0;border-radius:99px;outline:none;\
background-color:rgba(215,231,247,.14);background-image:linear-gradient(90deg,#5CB2FF,#3B7FF0);\
background-repeat:no-repeat;background-size:var(--fill,0%) 100%;cursor:pointer;\
}\
input[type='range']::-webkit-slider-runnable-track { height:8px;background:transparent; }\
input[type='range']::-webkit-slider-thumb {\
-webkit-appearance:none;appearance:none;width:24px;height:24px;margin-top:-8px;border-radius:50%;\
background:#F5FBFF;border:3px solid #6CB8FF;box-shadow:0 2px 8px rgba(0,0,0,.45);cursor:pointer;\
}\
input[type='range']::-moz-range-track { height:8px;background:transparent; }\
input[type='range']::-moz-range-progress { height:8px;border-radius:99px;background:linear-gradient(90deg,#5CB2FF,#3B7FF0); }\
input[type='range']::-moz-range-thumb { width:18px;height:18px;border-radius:50%;background:#F5FBFF;border:3px solid #6CB8FF;box-shadow:0 2px 8px rgba(0,0,0,.45);cursor:pointer; }\
.button-link,.button,.popup-button,.send-color-button,.advanced-action,.advanced-primary,.maintenance-button,.error-button {\
display:inline-flex;align-items:center;justify-content:center;min-height:44px;max-width:100%;margin:6px 2px;padding:9px 18px;\
border:1px solid var(--line);border-radius:var(--r-m);background:var(--glass);color:var(--text);\
font-family:inherit;font-size:14px;font-weight:650;line-height:1.2;text-align:center;text-decoration:none;cursor:pointer;\
box-shadow:inset 0 1px 0 rgba(255,255,255,.08);transition:background .16s,border-color .16s,transform .1s,box-shadow .16s;\
}\
.button-link,.button { flex:1; }\
.button-link:hover,.maintenance-button:hover,.advanced-action:hover { background:var(--glass-hover);border-color:var(--line-strong); }\
.button,.popup-button,.send-color-button,.advanced-primary,.advanced-action.primary {\
border-color:rgba(150,205,255,.5);background:var(--blue-grad);color:#FFFFFF;text-shadow:0 1px 2px rgba(10,30,70,.35);\
box-shadow:0 8px 20px rgba(59,127,240,.28),inset 0 1px 0 rgba(255,255,255,.3);\
}\
.button:hover,.popup-button:hover,.send-color-button:hover,.advanced-primary:hover,.advanced-action.primary:hover {\
border-color:rgba(190,224,255,.7);background:linear-gradient(135deg,#72BEFF 0%,#4B8DF5 100%);\
}\
.error-button,.advanced-action.danger,.maintenance-button.danger {\
border-color:rgba(255,138,143,.5);background:rgba(240,106,114,.15);color:#FFDADC;box-shadow:inset 0 1px 0 rgba(255,255,255,.06);text-shadow:none;\
}\
.error-button:hover,.advanced-action.danger:hover,.maintenance-button.danger:hover { background:rgba(240,106,114,.26);border-color:rgba(255,160,165,.7); }\
.maintenance-button.update-button { border-color:rgba(242,198,109,.55);background:rgba(242,198,109,.14);color:#FFE9BD; }\
.maintenance-button.update-button:hover { background:rgba(242,198,109,.25);border-color:rgba(248,214,140,.75); }\
.button-link:active,.button:active,.popup-button:active,.maintenance-button:active,.error-button:active,.advanced-action:active,.advanced-primary:active { transform:translateY(1px); }\
.button-container,.controls { display:flex;flex-wrap:wrap;justify-content:center;gap:8px;width:100%;margin:8px auto; }\
.icon-btn {\
position:relative;display:inline-flex;align-items:center;justify-content:center;flex:1;min-height:44px;height:44px;margin-top:6px;padding:0 12px;\
border:1px solid var(--line);border-radius:var(--r-m);background:var(--glass);color:var(--text);\
font-size:1.25rem;cursor:pointer;box-shadow:inset 0 1px 0 rgba(255,255,255,.08);\
transition:background .16s,border-color .16s,transform .1s;\
}\
.icon-btn:hover { background:var(--glass-hover);border-color:var(--line-strong); }\
.icon-btn:active { transform:translateY(1px); }\
.icon-btn::before { content:'';display:inline-block; }\
.icon-btn.on::before { content:'ON';font-size:1rem;font-weight:700; }\
.icon-btn.off::before { content:'OFF';font-size:1rem;font-weight:700;color:var(--text-soft); }\
.icon-btn.play::before { content:'\\25B6';margin-left:2px; }\
.icon-btn.stop::before { content:'\\25A0';color:#FF9AA0; }\
.icon-btn.save::before { content:'\\1F4BE'; }\
.icon-btn.load::before { content:'\\1F4E5'; }\
.icon-btn.settings::before { content:'\\2699'; }\
.icon-btn.presets::before { content:'\\1F5C3'; }\
.icon-btn.ok::before { content:'\\2714'; }\
.icon-btn.ok,.icon-btn.save { border-color:rgba(150,205,255,.5);background:var(--blue-grad);color:#FFFFFF;box-shadow:0 8px 20px rgba(59,127,240,.28),inset 0 1px 0 rgba(255,255,255,.3); }\
.icon-btn.ok:hover,.icon-btn.save:hover { background:linear-gradient(135deg,#72BEFF 0%,#4B8DF5 100%); }\
.icon-btn.device-state { gap:10px;font-size:14px;font-weight:700; }\
.icon-btn.device-state::before { width:9px;height:9px;border-radius:50%;background:currentColor; }\
.icon-btn.state-enabled { border-color:rgba(111,186,255,.6);background:linear-gradient(135deg,rgba(92,178,255,.34),rgba(59,127,240,.2));color:#E4F2FF; }\
.icon-btn.state-enabled:hover { border-color:rgba(160,212,255,.8);background:linear-gradient(135deg,rgba(92,178,255,.44),rgba(59,127,240,.3)); }\
.icon-btn.state-enabled::before { color:#8DCBFF;box-shadow:0 0 0 4px rgba(141,203,255,.18),0 0 10px rgba(141,203,255,.7); }\
.icon-btn.state-disabled { color:var(--text-soft); }\
.icon-btn.state-disabled::before { color:#93A4B7; }\
.device-readouts { display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:9px;width:100%;margin:2px 0 4px; }\
.device-readout {\
display:flex;flex-direction:column;justify-content:center;min-height:54px;padding:9px 12px;\
background:var(--glass-well);border:1px solid var(--line-soft);border-radius:var(--r-m);\
}\
.device-readout:last-child:nth-child(odd) { grid-column:1 / -1; }\
.device-readout-label { color:var(--muted);font-size:10px;font-weight:700;letter-spacing:.08em;line-height:1.2;text-transform:uppercase; }\
.device-readout-value { margin-top:4px;color:var(--text);font-size:15px;font-weight:650;line-height:1.25;overflow-wrap:anywhere; }\
.device-readout-value.tone-accent { color:var(--blue-light); }\
.device-readout-value.tone-success { color:var(--success); }\
.device-readout-value.tone-warning { color:var(--warning); }\
.device-readout-value.tone-danger { color:var(--danger); }\
.color-picker { display:flex;flex-direction:column;align-items:center;margin-top:8px; }\
.color-input { width:64px;height:44px;margin-bottom:5px;padding:3px;border:1px solid var(--line);border-radius:var(--r-m);background:var(--glass-well); }\
.color-display { width:100%;height:10px;margin:6px 0 8px;border-radius:99px;transition:box-shadow .3s ease; }\
.color-display.on { box-shadow:0 0 16px 3px rgba(85,170,255,.38); }\
.color-display.off { box-shadow:none;opacity:.55; }\
.led-strip { display:flex;justify-content:flex-start;align-items:center;width:100%;min-width:0;padding:10px 0;overflow-x:auto; }\
.led { width:100%;height:20px;background-color:rgb(21,177,244); }\
.led.off { background-color:rgba(0,0,0,.1); }\
.ledContainer { width:100%;min-width:9px;padding:5px 0;background:var(--glass-well); }\
.ledContainer.marked { background:rgba(111,186,255,.75); }\
.sensor-error {\
display:flex;align-items:center;justify-content:center;min-height:64px;height:auto;margin-top:5px;padding:12px;\
border:1px solid rgba(255,138,143,.45);border-radius:var(--r-m);background:rgba(240,106,114,.1);color:#FFB3B7;font-weight:650;text-align:center;\
}\
.memory-bar,.software-update-progress {\
position:relative;width:100%;max-width:400px;height:10px;overflow:hidden;border:1px solid var(--line-soft);border-radius:99px;background:rgba(215,231,247,.1);\
}\
.software-update-progress { max-width:none;height:12px; }\
.memory-bar-fill,.software-update-progress-fill { width:0%;height:100%;border-radius:inherit;background:var(--blue-grad);transition:width .3s ease; }\
.software-update-selected {\
margin:0 0 12px;padding:12px 14px;border:1px solid rgba(111,186,255,.45);border-radius:var(--r-m);\
background:rgba(92,178,255,.12);color:#DCEEFF;font-weight:650;text-align:center;\
}\
.software-update-progress-value { margin-top:8px;color:var(--text);font-weight:650;text-align:center; }\
.container .loading-overlay {\
position:absolute;inset:0;z-index:10;display:flex;align-items:center;justify-content:center;max-width:100%;max-height:100%;padding:12px;\
overflow:hidden;border-radius:inherit;background:rgba(14,22,33,.92);pointer-events:none;\
}\
.loading-overlay .spinner { flex:0 0 auto;width:34px;height:34px;border:3px solid rgba(111,186,255,.2);border-top-color:var(--blue-1);border-radius:50%;animation:spin 1s linear infinite; }\
.loading-overlay .loading-text { min-width:0;margin-left:10px;color:var(--text);font-weight:650;overflow-wrap:anywhere; }\
.popup-overlay {\
position:fixed;inset:0;z-index:15;display:none;justify-content:center;align-items:center;padding:12px;\
background:rgba(5,9,15,.62);-webkit-backdrop-filter:blur(10px);backdrop-filter:blur(10px);transition:opacity .3s ease;\
}\
.visible-popup { display:flex;opacity:1; }\
.hidden-popup { display:none;opacity:0; }\
.popup-content {\
position:relative;display:grid;grid-template-rows:auto minmax(0,1fr);width:min(440px,100%);max-height:min(680px,calc(100dvh - 24px));padding:20px;\
background:linear-gradient(145deg,rgba(44,66,94,.94) 0%,rgba(19,30,45,.96) 100%);border:1px solid var(--line-strong);border-radius:24px;\
box-shadow:0 24px 60px rgba(0,0,0,.55),inset 0 1px 0 rgba(255,255,255,.1);text-align:center;\
transform:translateY(8px) scale(.98);transition:transform .3s ease;\
-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;\
}\
.popup-content.show { transform:translateY(0) scale(1); }\
.popup-header {\
min-width:0;padding:0 40px 14px;border-bottom:1px solid var(--line-soft);\
color:var(--blue-light);font-size:18px;font-weight:650;overflow:hidden;text-overflow:ellipsis;white-space:nowrap;\
}\
.popup-message { min-width:0;padding-top:14px;color:var(--text-soft);font-size:15px;line-height:1.45;overflow:auto;overscroll-behavior:contain; }\
.popup-close {\
position:absolute;top:12px;right:12px;display:flex;align-items:center;justify-content:center;width:32px;height:32px;\
border:1px solid var(--line);border-radius:50%;background:rgba(255,255,255,.08);color:var(--text);\
font-size:20px;line-height:1;cursor:pointer;transition:background .16s,border-color .16s;\
}\
.popup-close:hover { background:rgba(240,106,114,.3);border-color:rgba(255,138,143,.6); }\
.popup-password-input { width:100%;min-height:46px;margin:0;padding:10px 14px;border:1px solid var(--line);border-radius:var(--r-m);background:var(--glass-well);color:var(--text);font-size:16px; }\
#popup-content,#password-popup-content { display:flex;flex-direction:column;align-items:stretch;gap:16px; }\
#popup-content .popup-header,#password-popup-content .popup-header { margin:0; }\
#popup-content .popup-message { padding:4px 0;overflow:visible;text-align:center; }\
#popup-content .popup-button { align-self:center;width:min(100%,220px);height:auto;margin:0;padding:11px 18px;font-size:14px;line-height:1.2; }\
#password-popup-content { gap:14px; }\
#password-popup-content .popup-message { padding:0;overflow:visible;text-align:center; }\
#password-popup-content .popup-button { align-self:stretch;width:100%;height:auto;margin:0;line-height:1.2; }\
.popup-backdrop { position:fixed;top:0;left:0;z-index:9;display:none;width:100%;height:100%;background:rgba(5,9,15,.6); }\
.color-picker-popup {\
position:fixed;top:25%;z-index:16;display:none;flex-direction:column;align-items:center;padding:20px;\
background:linear-gradient(145deg,rgba(44,66,94,.94),rgba(19,30,45,.96));color:var(--blue-light);\
border:1px solid var(--line-strong);border-radius:var(--r-xl);box-shadow:0 24px 60px rgba(0,0,0,.5);\
}\
.popup-buttons { display:flex;gap:10px; }\
.popup-buttons button { padding:8px 14px;border:1px solid rgba(150,205,255,.5);border-radius:var(--r-s);background:var(--blue-grad);color:#FFFFFF;font-size:15px;cursor:pointer; }\
.notifications-popup {\
position:fixed;top:50%;left:50%;z-index:11;display:none;width:80%;max-width:400px;padding:20px;transform:translate(-50%,-50%);\
background:rgba(22,34,49,.96);color:var(--text-soft);border:1px solid var(--line);border-radius:var(--r-xl);box-shadow:0 24px 60px rgba(0,0,0,.5);\
}\
.notification { display:flex;flex-direction:column;margin-bottom:10px;padding:10px 12px;border:1px solid;border-radius:var(--r-m);background:var(--glass-well)!important; }\
.notification-title { margin-bottom:5px;font-weight:700; }\
.notification.error { border-color:rgba(255,138,143,.55);color:#FFB3B7; }\
.notification.warning { border-color:rgba(242,198,109,.55);color:var(--warning); }\
.notification.info { border-color:rgba(111,186,255,.55);color:var(--blue-light); }\
.settings-section-title,.settings-menu-title { margin:4px 0 2px;color:var(--muted);font-size:11px;font-weight:700;letter-spacing:.08em;text-align:center;text-transform:uppercase; }\
.settings-menu-title { margin:0 0 5px; }\
.settings-menu-section { display:flex;flex-direction:column;align-items:stretch;width:min(100%,460px);margin:14px auto 0;padding-top:12px;border-top:1px solid var(--line-soft); }\
.maintenance-button { align-self:center;width:min(100%,300px); }\
.container > form { display:flex;flex-direction:column;align-items:stretch;gap:8px; }\
.container > form > .header,.container > form > label,.container > form > hr,.container > form > br,.container > form > .access-level-hidder { grid-column:1 / -1; }\
.container > form > .access-level-hidder label { width:100%; }\
.container > form > .button-link { align-self:center;width:min(100%,300px);margin:0; }\
.container > form > .error-button { align-self:flex-start;margin:0; }\
.access-level-hidder { visibility:hidden;display:none; }\
.device-container { opacity:0;max-height:1100px;overflow:hidden;animation:fadeInSlide .5s forwards;transition:max-height .5s ease-in-out,padding .5s ease-in-out,background-color .3s; }\
.device-container.disabled { min-height:58px;max-height:58px;padding:10px 16px;background:rgba(255,255,255,.025); }\
.device-container .slot-header { flex:0 0 auto;width:100%;min-height:36px;margin:0;gap:10px;color:var(--text);white-space:nowrap; }\
.device-container .slot-header .status-text { margin-left:auto;color:var(--muted); }\
.extra-fields { max-height:0;overflow:hidden;opacity:0;transition:max-height 1.5s ease,opacity 1.5s ease; }\
.visible { max-height:900px;opacity:1; }\
.table-graphite {\
display:block;width:100%;max-width:1180px;margin:4px auto 10px;overflow-x:auto;overscroll-behavior-inline:contain;\
background:var(--glass);border:1px solid var(--line);border-radius:var(--r-l);box-shadow:var(--shadow);font-family:inherit;color:var(--text);\
}\
.table-graphite thead,.table-graphite tbody { display:table;width:max-content;min-width:100%;table-layout:auto; }\
.table-graphite th,.table-graphite td { min-width:140px;padding:12px;border-bottom:1px solid var(--line-soft);text-align:left;vertical-align:middle; }\
.table-graphite th:first-child,.table-graphite td:first-child { min-width:96px; }\
.table-graphite thead { background:rgba(255,255,255,.06); }\
.table-graphite thead th { border-bottom:1px solid rgba(111,186,255,.5);color:var(--muted);font-size:11px;letter-spacing:.06em;text-transform:uppercase;white-space:normal; }\
.table-graphite tbody tr:nth-child(even) { background:rgba(255,255,255,.025); }\
.table-graphite tbody tr:hover { background:rgba(255,255,255,.06); }\
.table-graphite input,.table-graphite select { width:100%;min-width:120px; }\
.table-graphite .button,.table-graphite .error-button { width:100%;margin:0; }\
.table-graphite .badge { display:inline-block;padding:2px 9px;border-radius:99px;background:var(--blue-grad);color:#FFFFFF;font-size:.75em; }\
.table-graphite .badge.inactive { background:rgba(255,255,255,.14); }\
.cell-conditions { display:grid;gap:6px; }\
.rtc-interval label,.rtc-days label { margin:6px 0; }\
.rtc-days { display:flex;flex-wrap:wrap;gap:5px; }\
.rtc-days label {\
display:inline-flex;align-items:center;justify-content:flex-start;gap:5px;min-width:52px;padding:6px 8px;\
background:var(--glass-well);border:1px solid var(--line-soft);border-radius:var(--r-s);color:var(--text);white-space:nowrap;\
}\
.rtc-days input[type='checkbox'] { display:block;visibility:visible;opacity:1;width:20px;height:20px;margin:0; }\
.advanced-controls-form { display:flex;flex-direction:column;gap:14px;padding:6px 2px 2px;text-align:left; }\
.advanced-status-card { display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:8px;padding:12px;background:var(--glass-well);border:1px solid var(--line-soft);border-radius:var(--r-m); }\
.advanced-status-item { display:flex;flex-direction:column;gap:3px;min-width:0; }\
.advanced-status-item.wide { grid-column:1 / -1; }\
.advanced-status-label { color:var(--muted);font-size:10px;font-weight:700;letter-spacing:.08em;text-transform:uppercase; }\
.advanced-status-value { color:var(--text);font-size:14px;font-weight:650;overflow-wrap:anywhere; }\
.advanced-status-value.good { color:var(--success); }\
.advanced-status-value.warn { color:var(--warning); }\
.advanced-status-value.danger { color:var(--danger); }\
.advanced-field { display:grid;grid-template-columns:minmax(0,1fr) minmax(150px,1.25fr);gap:14px;align-items:center;margin:0; }\
.advanced-field > span { color:var(--text-soft);font-size:14px; }\
.advanced-field select,.advanced-field input[type='number'] { width:100%;margin:0; }\
.advanced-switch { display:flex;align-items:center;justify-content:space-between;min-height:44px;margin:0;padding:0 14px;background:var(--glass-well);border:1px solid var(--line-soft);border-radius:var(--r-m);color:var(--text-soft); }\
.advanced-switch input[type='checkbox'] { order:2; }\
.advanced-actions { display:grid;grid-template-columns:repeat(3,1fr);gap:9px;margin-top:2px; }\
.advanced-actions .advanced-action { margin:0; }\
.advanced-primary { width:100%;margin-top:2px; }\
footer { padding:2px 0 6px;color:#7B8CA1;text-align:center;font-size:11px; }\
footer a { margin:0 4px;color:var(--blue-light);text-decoration:none; }\
@media (max-width:520px) {\
body { padding-top:10px;font-size:14px; }\
.wrapper { gap:12px; }\
#rooms { gap:12px; }\
body .room-container { padding:14px 10px 10px; }\
body .container,body .device-container { width:100%;padding:14px; }\
body .device-container label { gap:8px; }\
body .device-container input[type='number'],body .device-container select,body .device-container input[type='text'] { width:min(190px,58%); }\
body .table-graphite { display:block;overflow:visible;border:0;border-radius:var(--r-m);background:transparent;box-shadow:none; }\
body .table-graphite thead { display:none; }\
body .table-graphite tbody { display:grid;width:100%;min-width:0;gap:10px; }\
body .table-graphite tbody tr { display:grid;grid-template-columns:1fr;gap:8px;padding:12px;background:var(--glass);border:1px solid var(--line);border-radius:var(--r-l); }\
body .table-graphite th,body .table-graphite td { display:grid;grid-template-columns:minmax(92px,.42fr) minmax(0,1fr);align-items:center;min-width:0;padding:0;border:0;text-align:left; }\
body .table-graphite td::before { color:var(--muted);font-size:11px;font-weight:700;letter-spacing:.04em;text-transform:uppercase; }\
body .table-graphite td > input,body .table-graphite td > select,body .table-graphite td > button,body .table-graphite td > div { width:100%;min-width:0; }\
body #data-table td:nth-child(1)::before { content:'Mapping ID'; }\
body #data-table td:nth-child(2)::before { content:'Event ID'; }\
body #data-table td:nth-child(3)::before { content:'Device'; }\
body #data-table td:nth-child(4)::before { content:'Action'; }\
body #data-table td:nth-child(5)::before { content:'Conditions'; }\
body #data-table td:nth-child(6)::before { content:''; }\
body #condition-table td:nth-child(1)::before { content:'Condition ID'; }\
body #condition-table td:nth-child(2)::before { content:'Device'; }\
body #condition-table td:nth-child(3)::before { content:'Condition'; }\
body #condition-table td:nth-child(4)::before { content:''; }\
body #rtc-event-table td:nth-child(1)::before { content:'Event ID'; }\
body #rtc-event-table td:nth-child(2)::before { content:'Repeats'; }\
body #rtc-event-table td:nth-child(3)::before { content:'Schedule'; }\
body #rtc-event-table td:nth-child(4)::before { content:''; }\
body #unmapped-event-table td:nth-child(1)::before { content:'Source'; }\
body #unmapped-event-table td:nth-child(2)::before { content:'Event ID'; }\
body #unmapped-event-table td:nth-child(3)::before { content:''; }\
body .table-graphite td:last-child:has(.error-button),body .table-graphite td:last-child:has(.button) { display:block; }\
body .table-graphite td:last-child::before { display:none; }\
body .rtc-weekly { min-width:0; }\
body .rtc-days { display:grid;grid-template-columns:repeat(4,minmax(0,1fr));gap:5px; }\
body .rtc-days label { min-width:0;margin:0;padding:6px 4px;font-size:12px; }\
body .wrapper > .button,body .wrapper > .button-link { width:min(100%,420px);flex:none; }\
.container > form > .button-link,.container > form > .error-button,.maintenance-button { width:100%;align-self:stretch; }\
.popup-overlay { padding:8px;align-items:center; }\
.popup-content { width:100%;max-height:calc(100dvh - 16px);padding:16px; }\
.popup-header { padding-bottom:11px;font-size:17px; }\
.popup-message { padding-top:12px; }\
.advanced-field { grid-template-columns:1fr;gap:6px; }\
.advanced-actions { grid-template-columns:1fr; }\
}\
</style>\
";

#endif
