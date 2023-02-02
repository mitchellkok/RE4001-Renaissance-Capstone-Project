import React, { useState, useEffect } from 'react';
import {io} from 'socket.io-client';
import Chart from './chart';

import Button from 'react-bootstrap/Button';
import Modal from 'react-bootstrap/Modal';
import 'bootstrap/dist/css/bootstrap.min.css';

import './bootstrap_modal.css';
import styles from './fetch.module.css';

const DataTable = () => {
  const [data, setData] = useState({});
  const [cumulative, setDataArray] = useState([]);

  // For Modal
  const [modaltitle, setModalTitle] = useState("Title");
  const [charttitle, setChartTitle] = useState("Title");
  const [modalshow, setModalShow] = useState(false);
  const [datakey, setKey] = useState("");
  const handleClose = () => setModalShow(false);
  const handleShow = (newmodaltitle, newcharttitle, datakey)  => (event) => {
    setModalShow(true);
    setModalTitle(newmodaltitle);
    setChartTitle(newcharttitle);
    setKey(datakey);
  }

  useEffect(() => {
      fetch("http://localhost:8090/begin")
  }, []);

  useEffect(() => {
    const socket = io('http://localhost:8090'); // replace with your server's URL
    socket.on('connect', () => {
      console.log('Connected to WebSocket server');
    });
    socket.on('data', (data) => {
      setData(data);
      setDataArray((cumulative) => [...cumulative, data]);
      console.log(data);
    });
  }, []);

  return (
    <>
      <div>
          {(typeof data.date_time === 'undefined') ? (
              <p style={{color: 'White'}}>Loading...</p>
          ) : (
              <div id="load"> 
                  <h1 style={{color: 'White', margin:"2rem"}}>{ data.date_time }</h1>
                  <div className={styles.row}>
                    <div className={styles.midcolumn25}>
                      <div className={styles.column25}>
                        <h3 className={styles.h3_custom} >Gas Sensors</h3>
                        <div className={styles.reading} onClick={handleShow("DFRobot Gravity SO2", "Gravity SO2", "g_so2")}>
                          <h5 className={styles.h5_custom}>Gravity SO2</h5>
                          <h5>{ data.g_so2.toFixed(2) } ppm</h5>
                          <h5>{ data.g_t.toFixed(2) } °C</h5>
                        </div>
                        <div className={styles.reading} onClick={handleShow("ECSense SO2", "ECSense SO2", "e_so2")}>
                          <h5 className={styles.h5_custom}>ECSense SO2</h5>
                          <h5>{ data.g_so2.toFixed(2) } ppm</h5>
                        </div>
                        <div className={styles.reading} onClick={handleShow("CO2", "CO2", "co2")}>
                          <h5 className={styles.h5_custom}>CO2</h5>
                          <h5>{ data.g_so2.toFixed(2) } ppm</h5>
                        </div>
                      </div>
                      <div className={styles.column25}>
                        <h3 className={styles.h3_custom} >Others</h3>
                        <div className={styles.reading} onClick={handleShow("Thermocouple", "Thermocouple", "t_t")}>
                          <h5 className={styles.h5_custom}>Thermocouple</h5>
                          <h5>{ data.t_t.toFixed(2) } °C</h5>
                        </div>
                        <div className={styles.reading} onClick={handleShow("LoRa RSSI", "RSSI", "rssi")}>
                          <h5 className={styles.h5_custom}>LoRa RSSI</h5>
                          <h5>{ data.t_t.toFixed(2) } °C</h5>
                        </div>
                      </div>
                    </div>
                    <div className={styles.midcolumn25}>
                      <div className={styles.column25}>
                        <div>
                          <h3 className={styles.h3_custom} >Internal 3 in 1</h3>
                          <div className={styles.reading} onClick={handleShow("Master 3 in 1", "Temperature", "m_t")}>
                            <h5 className={styles.h5_custom}>Temperature</h5>
                            <h5>{ data.m_t.toFixed(2) } °C</h5>
                          </div>
                          <div className={styles.reading} onClick={handleShow("Master 3 in 1", "Pressure", "m_p")}>
                            <h5 className={styles.h5_custom}>Pressure</h5>
                            <h5>{ data.m_p.toFixed(2) } hPa</h5>
                          </div>
                          <div className={styles.reading} onClick={handleShow("Master 3 in 1", "Humidity", "m_h")}>
                            <h5 className={styles.h5_custom}>Humidity</h5>
                            <h5>{ data.m_h.toFixed(2) } %rH</h5>
                          </div>
                          <h3 className={styles.h3_custom} >External 3 in 1</h3>
                          <div className={styles.reading} onClick={handleShow("Slave 3 in 1", "Temperature", "s_t")}>
                            <h5 className={styles.h5_custom}>Temperature</h5>
                            <h5>{ data.s_t.toFixed(2) } °C</h5>
                          </div>
                          <div className={styles.reading} onClick={handleShow("Slave 3 in 1", "Pressure", "s_p")}>
                            <h5 className={styles.h5_custom}>Pressure</h5>
                            <h5>{ data.s_p.toFixed(2) } hPa</h5>
                          </div>
                          <div className={styles.reading} onClick={handleShow("Slave 3 in 1", "Humidity", "s_h")}>
                            <h5 className={styles.h5_custom}>Humidity</h5>
                            <h5>{ data.s_h.toFixed(2) } %rH</h5>
                          </div>
                        </div>  
                      </div>
                    </div>
                    <div className={styles.midcolumn50}>
                      <div className={styles.column50}>
                        <h3 className={styles.h3_custom} >GPS</h3>
                        <div className={styles.subrow}>
                          <div className={styles.subcolumn}>
                            <div className={styles.reading} onClick={handleShow("GPS", "Satellites", "gps_sat")}>
                              <h5 className={styles.h5_custom}>Satellites</h5>
                              <h5>{ data.t_t.toFixed(0) }</h5>
                            </div>
                            <div className={styles.reading} onClick={handleShow("GPS", "HDOP", "gps_hdop")}>
                              <h5 className={styles.h5_custom}>HDOP</h5>
                              <h5>{ data.t_t.toFixed(2) }</h5>
                            </div>
                            <div className={styles.reading} onClick={handleShow("GPS", "Latitude", "gps_lat")}>
                              <h5 className={styles.h5_custom}>Latitude</h5>
                              <h5>{ data.t_t.toFixed(2) } °</h5>
                            </div>
                            <div className={styles.reading} onClick={handleShow("GPS", "Longitude", "gps_lng")}>
                              <h5 className={styles.h5_custom}>Longitude</h5>
                              <h5>{ data.t_t.toFixed(2) } °</h5>
                            </div>
                          </div>
                          <div className={styles.subcolumn}>
                            <div className={styles.reading} onClick={handleShow("GPS", "Altitude", "gps_alt")}>
                              <h5 className={styles.h5_custom}>Altitude</h5>
                              <h5>{ data.t_t.toFixed(2) } m</h5>
                            </div>
                            <div className={styles.reading} onClick={handleShow("GPS", "Course", "gps_deg")}>
                              <h5 className={styles.h5_custom}>Course</h5>
                              <h5>{ data.t_t.toFixed(2) } °</h5>
                            </div>
                            <div className={styles.reading} onClick={handleShow("GPS", "Ground Speed", "gps_mps")}>
                              <h5 className={styles.h5_custom}>Ground Speed</h5>
                              <h5>{ data.t_t.toFixed(2) }</h5>
                            </div>
                          </div>
                        </div>
                      </div>
                      <div className={styles.column50}>
                        <h3 className={styles.h3_custom} >IMU</h3>
                        <div className={styles.subrow}>
                          <div style={{width:'14%'}} className={styles.reading} onClick={handleShow("IMU", "Temperature", "imu_tmp")}>
                            <h5 className={styles.h5_custom}>Temp</h5>
                            <h5>{ data.t_t.toFixed(1) } <br></br>°C</h5>
                          </div>
                          <div style={{width:'14%'}} className={styles.reading} onClick={handleShow("IMU", "Acceleration X", "imu_acx")}>
                            <h5 className={styles.h5_custom}>Acc-X</h5>
                            <h5>{ data.t_t.toFixed(1) } </h5>
                          </div>
                          <div style={{width:'14%'}} className={styles.reading} onClick={handleShow("IMU", "Acceleration Y", "imu_acy")}>
                            <h5 className={styles.h5_custom}>Acc-Y</h5>
                            <h5>{ data.t_t.toFixed(1) } </h5>
                          </div>
                          <div style={{width:'14%'}} className={styles.reading} onClick={handleShow("IMU", "Acceleration Z", "imu_acz")}>
                            <h5 className={styles.h5_custom}>Acc-Z</h5>
                            <h5>{ data.t_t.toFixed(1) } </h5>
                          </div>
                          <div style={{width:'14%'}} className={styles.reading} onClick={handleShow("IMU", "Gyro X", "imu_gyx")}>
                            <h5 className={styles.h5_custom}>Gyr-Z</h5>
                            <h5>{ data.t_t.toFixed(1) } </h5>
                          </div>
                          <div style={{width:'14%'}} className={styles.reading} onClick={handleShow("IMU", "Gyro Y", "imu_gyy")}>
                            <h5 className={styles.h5_custom}>Gyr-Y</h5>
                            <h5>{ data.t_t.toFixed(1) } </h5>
                          </div>
                          <div style={{width:'14%'}} className={styles.reading} onClick={handleShow("IMU", "Gyro Z", "imu_gyz")}>
                            <h5 className={styles.h5_custom}>Gyr-Z</h5>
                            <h5>{ data.t_t.toFixed(1) } </h5>
                          </div>
                        </div>
                      </div>
                    </div>
                    
                  </div>
              </div>
          )}  
      </div>

      <Modal 
        show={modalshow} 
        onHide={handleClose} 
        dialogClassName="modal-90w"  
        centered
      >
        <Modal.Header closeButton>
          <Modal.Title>{modaltitle}</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <Chart charttitle={charttitle} datakey={datakey} cumulative={cumulative} />
        </Modal.Body>
        <Modal.Footer>
          <Button variant="secondary" onClick={handleClose}>
            Close
          </Button>
        </Modal.Footer>
      </Modal>
    </>
  );
};

export default DataTable;