import React, { useState, useEffect } from 'react';
import {io} from 'socket.io-client';
import styles from './fetch.module.css';

const DataTable = () => {
    const [data, setData] = useState({});

//   useEffect(() => {
//     fetch('/data')
//       .then(response => response.json())
//       .then(data => {
//         setData(data)
//         console.log(data)
//       });
//   }, []);

  useEffect(() => {
    const socket = io('http://localhost:8090'); // replace with your server's URL
    socket.on('connect', () => {
      console.log('Connected to WebSocket server');
    });
    socket.on('data', (data) => {
      setData(data);
      console.log(data)
    });
  }, []);

  return (
        <div>
            {(typeof data.date_time === 'undefined') ? (
                <p style={{color: 'White'}}>Loading...</p>
            ) : (
                <div id="load"> 
                    <h1 style={{color: 'White'}}>{ data.date_time }</h1>
                    <div className={styles.row}>
                    <div className={styles.column}>
                        <h1>Master 3 in 1</h1>
                        <div className={styles.reading}>
                        <h2 className={styles.h2_custom}>Temperature</h2>
                        <h1>{ data.m_t } °C</h1>
                        </div>
                        <div className={styles.reading}>
                        <h2 className={styles.h2_custom}>Pressure</h2>
                        <h1>{ data.m_p } hPa</h1>
                        </div>
                        <div className={styles.reading}>
                        <h2 className={styles.h2_custom}>Humidity</h2>
                        <h1>{ data.m_h } %rH</h1>
                        </div>
                    </div>
                    <div className={styles.column}>
                        <h1>Slave 3 in 1</h1>
                        <div className={styles.reading}>
                        <h2 className={styles.h2_custom}>Temperature</h2>
                        <h1>{ data.s_t } °C</h1>
                        </div>
                        <div className={styles.reading}>
                        <h2 className={styles.h2_custom}>Pressure</h2>
                        <h1>{ data.s_p } hPa</h1>
                        </div>
                        <div className={styles.reading}>
                        <h2 className={styles.h2_custom}>Humidity</h2>
                        <h1>{ data.s_h } %rH</h1>
                        </div>
                    </div>
                    <div className={styles.column}>
                        <h1>Other Sensors</h1>
                        <div className={styles.reading}>
                        <h2 className={styles.h2_custom}>Gravity SO2</h2>
                        <h1>{ data.g_so2 }  ppm</h1>
                        <h1>{ data.g_t }  °C</h1>
                        </div>
                        <div className={styles.reading}>
                        <h2 className={styles.h2_custom}>Thermocouple</h2>
                        <h1>{ data.t_t }  °C</h1>
                        </div>
                    </div>
                    </div>
                </div>
            )}  
        </div>
  );
};

export default DataTable;
