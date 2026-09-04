#ifndef TEMP_GAUGE_CSS_H
#define TEMP_GAUGE_CSS_H

/**
 * @file include/os/app/http/tempGauge/tempGaugeCSS.h
 * @brief Embedded web UI asset used by the Home Control Station HTTP interface.
 */



const char* tempGaugeCSS = "\
<style>\
        .temperature-container { display:grid; grid-template-columns:repeat(2,minmax(0,1fr)); gap:10px; width:100%; min-width:0; margin:0 auto; }\
        .temperature-widget, .humidity-widget { min-width:0; padding:10px 11px 8px; background:#1D232B; border:1px solid #3B4654; border-radius:6px; color:#E7ECF3; }\
        .meter-heading { display:flex; align-items:baseline; justify-content:space-between; gap:5px; min-width:0; margin-bottom:9px; }\
        .meter-label { min-width:0; overflow:hidden; color:#98A6B8; font-size:10px; font-weight:600; letter-spacing:.04em; text-overflow:ellipsis; text-transform:uppercase; white-space:nowrap; }\
        .temperature-value, .value-display { flex:0 0 auto; color:#F2F5F8; font-size:17px; font-weight:700; line-height:1; white-space:nowrap; }\
        .temperature-value { color:#FF796B; }\
        .humidity-value { color:#45B9DE; }\
        .temperature-meter, .humidity-meter { position:relative; height:10px; overflow:hidden; background:#2C3540; border-radius:5px; }\
        .temperature-fill { width:0%; height:100%; background:linear-gradient(90deg,#45B9DE 0%,#F3C969 52%,#FF796B 100%); border-radius:inherit; transition:width .25s ease; }\
        .temperature-marker { position:absolute; top:50%; left:0%; width:8px; height:8px; border:2px solid #F2F5F8; border-radius:50%; background:#FF796B; transform:translate(-50%,-50%); transition:left .25s ease; }\
        .humidity-fill { width:0%; height:100%; background:#45B9DE; border-radius:inherit; box-shadow:0 0 10px rgba(69,185,222,.35); transition:width .25s ease; }\
        .meter-scale { display:flex; justify-content:space-between; margin-top:5px; color:#7F8C9D; font-size:9px; line-height:1; }\
    </style>\
";


#endif