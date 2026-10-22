#ifndef TEMP_GAUGE_CSS_H
#define TEMP_GAUGE_CSS_H

/**
 * @file include/os/app/http/tempGauge/tempGaugeCSS.h
 * @brief Embedded web UI asset used by the Home Control Station HTTP interface.
 */



const char* tempGaugeCSS = "\
<style>\
        .temperature-container { display:grid; grid-template-columns:repeat(2,minmax(0,1fr)); gap:10px; width:100%; min-width:0; margin:0 auto; }\
        .temperature-widget, .humidity-widget { min-width:0; padding:11px 12px 9px; background:rgba(8,14,22,.42); border:1px solid rgba(169,191,217,.13); border-radius:13px; color:#F2F7FF; }\
        .meter-heading { display:flex; align-items:baseline; justify-content:space-between; gap:5px; min-width:0; margin-bottom:10px; }\
        .meter-label { min-width:0; overflow:hidden; color:#9DACBF; font-size:10px; font-weight:700; letter-spacing:.08em; text-overflow:ellipsis; text-transform:uppercase; white-space:nowrap; }\
        .temperature-value, .value-display { flex:0 0 auto; color:#F2F7FF; font-size:18px; font-weight:700; line-height:1; white-space:nowrap; }\
        .temperature-value { color:#FF9B86; }\
        .humidity-value { color:#7FE0D8; }\
        .temperature-meter, .humidity-meter { position:relative; height:8px; overflow:hidden; background:rgba(215,231,247,.12); border-radius:99px; }\
        .temperature-fill { width:0%; height:100%; background:linear-gradient(90deg,#5CB2FF 0%,#F3C969 52%,#FF9B86 100%); border-radius:inherit; transition:width .25s ease; }\
        .temperature-marker { position:absolute; top:50%; left:0%; width:8px; height:8px; border:2px solid #F2F7FF; border-radius:50%; background:#FF9B86; transform:translate(-50%,-50%); transition:left .25s ease; }\
        .humidity-fill { width:0%; height:100%; background:linear-gradient(90deg,#5CB2FF 0%,#7FE0D8 100%); border-radius:inherit; box-shadow:0 0 10px rgba(127,224,216,.3); transition:width .25s ease; }\
        .meter-scale { display:flex; justify-content:space-between; margin-top:6px; color:#9DACBF; font-size:9px; line-height:1; }\
    </style>\
";


#endif