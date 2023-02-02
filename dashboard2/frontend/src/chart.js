import React, { useState, useEffect } from 'react';
import { LineChart, Line, XAxis, YAxis, Tooltip, Legend, CartesianGrid, ResponsiveContainer } from 'recharts';

export default function Chart(args) {
    // const [data, setData] = useState([]);

    // useEffect(() => {
    //     fetch("http://localhost:8090/chart")
    //     .then(response => response.json())
    //     .then(data => setData(data));
    // }, []);
    // console.log(data);
    
    return (
        <div>
            <h2>{args.charttitle}</h2>
            <ResponsiveContainer width='100%' height={400}>
                <LineChart width={600} height={300} data={args.cumulative} margin={{ top: 5, right: 20, bottom: 5, left: 0 }}>
                    <Line type="monotone" dataKey={args.datakey} stroke="#8884d8" isAnimationActive={false} />
                    <CartesianGrid stroke="#ccc" strokeDasharray="5 5" />
                    <XAxis dataKey="date_time" />
                    <YAxis dataKey={args.datakey} />
                    <Tooltip />
                    <Legend />
                </LineChart>
            </ ResponsiveContainer>
        </div>
    );
}
  