#include "server/frontend.h"

const char index_html[] = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <title>Налаштування термостата</title>
    <meta name="viewport"
          content="width=device-width, initial-scale=1"
          charset="UTF-8">
    <link rel="icon" href="data:,">
    <style>
        .main-box {
            margin-top: 3%;
            margin-left: 10%;
            margin-right: 10%;
        }

        .temperatureBox {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            /* margin: 2%; */
        }

        .valueBox {
            background-color: #0040FF;
            color: white;
            margin: .8%;
        }

        .confBox {
            background-color: #0DC9FF;
            color: white;
            margin: .8%;
        }

        .operationBox {
            background-color: #FF4819;
            color: white;
            margin: .8%;
            font-size: 18px;
            text-align: center;
            display: flex;
            justify-content: center;
            align-items: center;
        }

        .temperatureMetadataBox {
            display: grid;
            grid-column-start: 1;
            grid-column-end: 4;
            grid-row: 1;
            /* text-align: right; */
            font-size: 1.2vw;
            /* font-family: 'Segoe UI Light'; */
        }

        .styleTemperatureValue {
            /* font-size: 26px; */
            padding-top: 1.7%;
            font-size: 1.7vw;
            text-align: center;
        }

        .save_button {
            width: 100%;
            background-color: #fa6400;
            color: azure;
            align-items: center;
            top:50%;
            padding: 1%;
            margin-top: 1%;
            margin-bottom: 1.5%;
            border: 0px;
        }


        body {
            /* font-family: 'Segoe UI'; */
            font-family: Verdana, sans-serif;
            font-size: 2vw;
            margin-bottom: 5%;
        }

        /* span, b, p {
            font-family: Verdana, sans-serif;
            font-size: 1.2vw;
        } */

        h1 {
            text-align: center;
            font-size: 2.4vw;
        }

        table {
            width: 100%;
            border-collapse: collapse;
        }

        th {
            text-align: left;
            border-bottom: 1px solid #ddd;
        }

        td {
            padding-top: 5px;
            padding-bottom: 5px;
            width: 100%;
        }

        @media (max-width: 768px) {
            .temperatureBox  {
                grid-template-columns: 1fr;
            }
        }
    </style>
</head>

<body>
    <h1>Показники</h1>
    <div class="main-box" id="style-main">
        <div class="temperatureBox">
            <div class="temperatureMetadataBox">
                <p>Останній запит: <span id="last_query_date">00:00:00 01.01.2000</span></p>
            </div>
            <div class="temperatureMetadataBox" style="text-align: right;">
                <p>Останній запис: <span id="last_record_down_date">00:00:00 01.01.2000</p>
            </div>
            <div class="valueBox">
                <p class="styleTemperatureValue">
                    <b> <span id="temp_inside">0.00</span> °C</b>
                </p>
                <p style="text-align: center;">
                    Внутрішня температура
                </p>
            </div>
            <div class="valueBox">
                <p class="styleTemperatureValue">
                    <b><span id="temp_outside">0.00</span> °C</b>
                </p>
                <p style="text-align: center;">
                    Температура оточення
                </p>
            </div>
            <div class="valueBox">
                <p class="styleTemperatureValue">
                    <b><span id="temp_liquid">0.00</span> °C</b>
                </p>
                <p style="text-align: center;">
                    Температура води
                </p>
            </div>
            <div class="valueBox">
                <p class="styleTemperatureValue">
                    <b><span id="illumination">0.00</span> lx</b>
                </p>
                <p style="text-align: center;">
                    Освітлення
                </p>
            </div>
            <div class="valueBox">
                <p class="styleTemperatureValue">
                    <b><span id="water_flow">0.00</span> Л/год</b>
                </p>
                <p style="text-align: center;">
                    Швидкість потоку рідини
                </p>
            </div>
            <div class="valueBox">
                <p class="styleTemperatureValue">
                    <b><span id="log_code">0</span></b>
                </p>
                <p style="text-align: center;">
                    Код відладки
                </p>
            </div>
        </div>
        <br><br>

        <h1>Конфігурація</h1>
        <div class="temperatureBox">
            <div class="confBox">
                <p class="styleTemperatureValue">
                    <b>1 сек.</b>
                </p>
                <p style="text-align: center;">
                    Перевірка датчиків
                </p>
            </div>
            <div class="confBox">
                <p class="styleTemperatureValue">
                    <b><span id="thread_read_saving">$FORMAT_THREAD_SAVING$</span></b>
                </p>
                <p style="text-align: center;">
                    Інтервал збереження показників
                </p>
            </div>
            <div class="confBox">
                <p class="styleTemperatureValue">
                    <b><span id="temp_max">$TEMPERATURE_MAX$</span> °C</b>
                </p>
                <p style="text-align: center;">
                    Допустима верхня межа внутрішньої температури
                </p>
            </div>
            <div class="confBox">
                <p class="styleTemperatureValue">
                    <b><span id="temp_hysteresis">$TEMPERATURE_HISTERESYS$</span> °C</b>
                </p>
                <p style="text-align: center;">
                    Вікно гістерезису
                </p>
            </div>
            <div class="confBox">
                <p class="styleTemperatureValue">
                    <b><span id="coef_inertia">$COEF_INERTIA$</span></b>
                </p>
                <p style="text-align: center;">
                    Інерційність системи
                </p>
            </div>
        </div>
        <br><br>

        <h1>Зміна налаштувань</h1>
        <div>
            <form action="/conf">
                <table>
                    <tr>
                        <td><label >Інтервал запису даних (сек.):</label></td>
                        <td><input name="interval_data_sec" id="interval_data_sec"/></td>
                    </tr>
                    <tr>
                        <td><label>Максимальна допустима температура (°C):</label></td>
                        <td><input name="max_temp_c" id="max_temp_c"/></td>
                    </tr>
                    <tr>
                        <td><label>Вікно гістерезису (°C):</label></td>
                        <td><input name="hysteresis_c" id="hysteresis_c"/></td>
                    </tr>
                    <tr>
                        <td><label>Коефіцієнт інерційності:</label></td>
                        <td><input name="coef_inert" id="coef_inert"/></td>
                    </tr>
                </table>
                <div>
                    <input type="submit" value="Зберегти налаштування" class="save_button">
                </div>
            </form>
        </div>
        <div>
            <form action="/confDate">
                <table>
                    <tr>
                        <td><label>Встановити час:</label></td>
                        <td><input type=datetime-local step="1" value="12:30:00" name="date"/></td>
                    </tr>
                </table>

                <div>
                    <input type="submit" value="Зберегти час" class="save_button">
                </div>
            </form>
        </div>
        <br><br>

        <h1>Завантажити дані</h1>
        <div class="">
            <div>
                <div>
                    <input type="month"
                           id="record_date"
                           value="2023-11"
                           min="2023-11"
                           step="1"/>
                </div>
                <div>
                    <a href='#' id="download_href">Скачати</a>
                 </div>
            </div>

            <script>
                function test() {
                    var date = document.getElementById("record_date").value;
                    var filename = date.substring(0, date.indexOf("T")) + ".thd";
                    window.location.href = "/monitoring?filename=" + filename;
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", "/monitoring?filename=" + filename, true);
                    xhr.send(null);
                }

                document.getElementById("download_href").onclick = function() {
                    var date = document.getElementById("record_date").value;
                    var filename = date + ".thd";
                    document.getElementById("download_href").href="/monitoring?filename=" + filename;
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", "/monitoring?filename=" + filename, true);
                    xhr.send(null);
                };

                function submitForm(event) {
                    var intervalDataSec = document.getElementById("interval_data_sec").value;

                    var maxTempC = document.getElementById("max_temp_c").value;
                    var hysteresis = document.getElementById("hysteresis_c").value;
                    var coefInert = document.getElementById("coef_inert").value;

                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", "/conf?interval_data_sec=" + intervalDataSec + "&max_temp_c=" + maxTempC +
                                    "&hysteresis_c=" + hysteresis + "&coef_inert=" + coefInert, true);
                    xhr.send(null);
                }

                if (!!window.EventSource) {
                    var source = new EventSource('/events');

                    // ---- System ----
                    source.addEventListener('open', function(e) {
                        console.log("Events Connected");
                    }, false);

                    source.addEventListener('error', function(e) {
                        if (e.target.readyState != EventSource.OPEN) {
                            console.log("Events Disconnected");
                        }
                    }, false);

                    source.addEventListener('message', function(e) {
                        console.log("message", e.data);
                    }, false);

                    // ---- Date ----
                    source.addEventListener('last_query_date', function(e) {
                        console.log("Last query date ", e.data);
                        document.getElementById("last_query_date").innerHTML = e.data;
                    }, false);

                    source.addEventListener('last_record_down_date', function(e) {
                        console.log("Last record down date ", e.data);
                        document.getElementById("last_record_down_date").innerHTML = e.data;
                    }, false);

                    // ---- Parameters ----
                    source.addEventListener('temp_inside', function(e) {
                        console.log("Temperature inside ", e.data);
                        document.getElementById("temp_inside").innerHTML = e.data;
                    }, false);

                    source.addEventListener('temp_outside', function(e) {
                        console.log("Temperature outside ", e.data);
                        document.getElementById("temp_outside").innerHTML = e.data;
                    }, false);

                    source.addEventListener('temp_liquid', function(e) {
                        console.log("Temperature liquid ", e.data);
                        document.getElementById("temp_liquid").innerHTML = e.data;
                    }, false);

                    source.addEventListener('illumination', function(e) {
                        console.log("Illumination ", e.data);
                        document.getElementById("illumination").innerHTML = e.data;
                    }, false);

                    source.addEventListener('water_flow', function(e) {
                        console.log("water_flow ", e.data);
                        document.getElementById("water_flow").innerHTML = e.data;
                    }, false);

                    source.addEventListener('log_code', function(e) {
                        console.log("log_code ", e.data);
                        document.getElementById("log_code").innerHTML = e.data;
                    }, false);

                    // ---- Configuration ----
                    source.addEventListener('thread_read_saving', function(e) {
                        console.log("Thread read saving ", e.data);
                        document.getElementById("thread_read_saving").innerHTML = e.data;
                    }, false);

                    source.addEventListener('temp_max', function(e) {
                        console.log("Temp max ", e.data);
                        document.getElementById("temp_max").innerHTML = e.data;
                    }, false);

                    source.addEventListener('coef_inertia', function(e) {
                        console.log("Coefficient inertia ", e.data);
                        document.getElementById("coef_inertia").innerHTML = e.data;
                    }, false);

                    source.addEventListener('temp_hysteresis', function(e) {
                        console.log("Temperature hysteresis ", e.data);
                        document.getElementById("temp_hysteresis").innerHTML = e.data;
                    }, false);
                }
            </script>
        </div>
    </div>
</body>

</html>
)rawliteral";
