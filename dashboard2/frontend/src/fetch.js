import React, { useState, useEffect } from 'react';
import {io} from 'socket.io-client';
import Chart from './chart';

import Button from 'react-bootstrap/Button';
import Modal from 'react-bootstrap/Modal';
import 'bootstrap/dist/css/bootstrap.min.css';

import './bootstrap_modal.css';
import styles from './fetch.module.css';

import Reading from './components/Reading';

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
                        <Reading 
                        parentFunction = {handleShow("DFRobot Gravity SO2", "Gravity SO2", "g_so2")}
                        device = 'Gravity SO2'
                        reading1= {data.g_so2.toFixed(2)}
                        unit1 = 'ppm'
                        reading2 = {data.g_t.toFixed(2)}
                        unit2 = '°C'/>

                        <Reading 
                        parentFunction = {handleShow("ECSense SO2", "ECSensor SO2", "e_so2")}
                        device = 'ECSense SO2'
                        reading1= {data.e_so2.toFixed(2)}
                        unit1 = 'ppm'/>

                        <Reading 
                        parentFunction = {handleShow("CO2", "CO2", "co2")}
                        device = 'CO2'
                        reading1= {data.co2.toFixed(2)}
                        unit1 = 'ppm'/>

                      </div>

                      <div className={styles.column25}>
                        <h3 className={styles.h3_custom} >Others</h3>
                        <Reading 
                        parentFunction = {handleShow("Thermocouple", "Thermocouple", "t_t")}
                        device = 'Thermocouple'
                        reading1= {data.t_t.toFixed(2)}
                        unit1 = '°C'/>

                        <Reading 
                        parentFunction = {handleShow("LoRa RSSI", "RSSI", "rssi")}
                        device = 'LoRa RSSI'
                        reading1= {data.rssi.toFixed(2)}
                        unit1 = '°C'/>

                      </div>
                    </div>
                    <div className={styles.midcolumn25}>
                      <div className={styles.column25}>
                        <div>
                          <h3 className={styles.h3_custom} >Internal 3 in 1</h3>             
                        <Reading 
                        parentFunction = {handleShow("Master 3 in 1", "Temperature", "m_t")}
                        device = 'Temperature'
                        reading1= {data.m_t.toFixed(2)}
                        unit1 = '°C'/>
                        <Reading 
                        parentFunction = {handleShow("Master 3 in 1", "Pressure", "m_p")}
                        device = 'Pressure'
                        reading1= {data.m_p.toFixed(2)}
                        unit1 = 'hPa'/>
                        <Reading 
                        parentFunction = {handleShow("Master 3 in 1", "Humidity", "m_h")}
                        device = 'Humidity'
                        reading1= {data.m_h.toFixed(2)}
                        unit1 = '%rH'/>
                          
                        <h3 className={styles.h3_custom} >External 3 in 1</h3>
                        <Reading 
                        parentFunction = {handleShow("Master 3 in 1", "Temperature", "s_t")}
                        device = 'Temperature'
                        reading1= {data.s_t.toFixed(2)}
                        unit1 = '°C'/>
                        <Reading 
                        parentFunction = {handleShow("Master 3 in 1", "Pressure", "s_p")}
                        device = 'Pressure'
                        reading1= {data.s_p.toFixed(2)}
                        unit1 = 'hPa'/>
                        <Reading 
                        parentFunction = {handleShow("Master 3 in 1", "Humidity", "s_h")}
                        device = 'Humidity'
                        reading1= {data.s_h.toFixed(2)}
                        unit1 = '%rH'/>
                        </div>  
                      </div>
                    </div>

                    <div className={styles.midcolumn50}>
                      <div className={styles.column50}>
                        <h3 className={styles.h3_custom} >GPS</h3>
                        <div className={styles.subrow}>
                          <div className={styles.subcolumn}>

                          <Reading 
                          parentFunction = {handleShow("GPS", "Satellites", "gps_sat")}
                          device = 'Satellites'
                          reading1= {data.gps_sat.toFixed(2)}
                          unit1 = ''/>

                          <Reading 
                          parentFunction = {handleShow("GPS", "HDOP", "gps_hdop")}
                          device = 'HDOP'
                          reading1= {data.gps_hdop.toFixed(2)}
                          unit1 = ''/>

                          <Reading 
                          parentFunction = {handleShow("GPS", "Latitude", "gps_lat")}
                          device = 'Latitude'
                          reading1= {data.gps_lat.toFixed(2)}
                          unit1 = '°'/>

                          <Reading 
                          parentFunction = {handleShow("GPS", "Longitude", "gps_lng")}
                          device = 'Longitude'
                          reading1= {data.gps_lng.toFixed(2)}
                          unit1 = '°'/>
                          </div>

                          <div className={styles.subcolumn}>
                          <Reading 
                          parentFunction = {handleShow("GPS", "Altitude", "gps_alt")}
                          device = 'Altitiude'
                          reading1= {data.gps_alt.toFixed(2)}
                          unit1 = 'm'/>

                          <Reading 
                          parentFunction = {handleShow("GPS", "Course", "gps_deg")}
                          device = 'Course'
                          reading1= {data.gps_deg.toFixed(2)}
                          unit1 = '°'/>

                          <Reading 
                          parentFunction = {handleShow("GPS", "Ground Speed", "gps_mps")}
                          device = 'Ground Speed'
                          reading1= {data.gps_mps.toFixed(2)}
                          unit1 = ''/>
                          </div>
                        </div>
                      </div>

                      <div className={styles.column50}>
                        <h3 className={styles.h3_custom} >IMU</h3>
                        <div className={styles.subrow}>

                        <Reading
                          style = '14%'
                          parentFunction = {handleShow("IMU", "Temperature", "imu_tmp")}
                          device = 'Temp'
                          reading1= {data.imu_tmp.toFixed(2)}
                          unit1 = '°C'/>

                        <Reading
                          style = '14%'
                          parentFunction = {handleShow("IMU", "Acceleration X", "imu_acx")}
                          device = 'Acc-X'
                          reading1= {data.imu_acx.toFixed(2)}
                          unit1 = ''/>

                          <Reading
                          style = '14%'
                          parentFunction = {handleShow("IMU", "Acceleration Y", "imu_acy")}
                          device = 'Acc-Y'
                          reading1= {data.imu_acy.toFixed(2)}
                          unit1 = ''/>

                          <Reading
                          style = '14%'
                          parentFunction = {handleShow("IMU", "Acceleration Z", "imu_acz")}
                          device = 'Acc-Z'
                          reading1= {data.imu_acz.toFixed(2)}
                          unit1 = ''/>

                          <Reading
                          style = '14%'
                          parentFunction = {handleShow("IMU", "Gyro X", "imu_gyx")}
                          device = 'Gyro-X'
                          reading1= {data.imu_gyx.toFixed(2)}
                          unit1 = ''/>

                          <Reading
                          style = '14%'
                          parentFunction = {handleShow("IMU", "Gyro Y", "imu_gyy")}
                          device = 'Gyro-Y'
                          reading1= {data.imu_gyy.toFixed(2)}
                          unit1 = ''/>

                          <Reading
                          style = '14%'
                          parentFunction = {handleShow("IMU", "Gyro Z", "imu_gyz")}
                          device = 'Gyro-Z'
                          reading1= {data.imu_gyz.toFixed(2)}
                          unit1 = ''/>
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
