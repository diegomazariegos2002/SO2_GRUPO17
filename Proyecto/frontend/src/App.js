import React, { useEffect, useState } from 'react';
import axios from 'axios';
import { format } from 'date-fns';
import PieChart from './components/PieChart';
import ProcessTable from './components/ProcessTable';
import 'bootstrap/dist/css/bootstrap.min.css';

const columns = [
  {
    Header: 'PID',
    accessor: 'pid',
  },
  {
    Header: 'Nombre',
    accessor: 'name',
  },
  {
    Header: 'Memoria',
    accessor: 'memorySize',
  },
  {
    Header: 'Memoria (%)',
    accessor: 'memoryPercentage',
  },
];

const requestsColumns = [
  {
    Header: 'PID',
    accessor: 'pid',
  },
  {
    Header: 'Llamada',
    accessor: 'call',
  },
  {
    Header: 'Tamaño',
    accessor: 'size',
  },
  {
    Header: 'Fecha',
    accessor: 'date',
  },
];

const columnsTop = [
  {
    Header: 'PID',
    accessor: 'pid',
  },
  {
    Header: 'Nombre',
    accessor: 'name',
  },
  {
    Header: 'Memoria',
    accessor: 'size',
  },
  {
    Header: 'Memoria (%)',
    accessor: 'sizePor',
  },
];

const App = () => {
  const [processData, setProcessData] = useState([]);
  const [requestsData, setRequestsData] = useState([]);
  const [topData, setTopData] = useState([]);

  useEffect(() => {
    const fetchData = async () => {
      try {
        const [processResponse, requestsResponse, topResponse] = await Promise.all([
          axios.get('http://localhost:5000/proceso'),
          axios.get('http://localhost:5000/llamada'),
          axios.get('http://localhost:5000/top')
        ]);

        const formattedProcessData = processResponse.data.map(item => ({
          pid: item.pid,
          name: item.nombre,
          memorySize: `${item.size}MB`, 
          memoryPercentage: `${item.porcentaje}%`,
        }));

        const formattedRequestsData = requestsResponse.data.map(item => ({
          pid: item.pid,
          call: item.llamada,
          size: `${item.size}MB`, 
          date: format(new Date(item.fecha), 'dd/MM/yyyy, HH:mm:ss')
        }));

        const formattedTopData = topResponse.data.map(item => ({
          pid: item.pid,
          name: item.nombre,
          size: `${item.size}MB`, 
          sizePor: `${item.porcentaje}%`,
        }));

        setProcessData(formattedProcessData);
        setRequestsData(formattedRequestsData);
        setTopData(formattedTopData);

      } catch (error) {
        console.error('Error fetching data', error);
      }
    };

    fetchData();
  }, []);

  return (
    <div className="container">
      <h1>Dashboard</h1>
      <div className="row">
        <div className="col-md-8">
          <h2>Top 10</h2>
          <ProcessTable columns={columnsTop} data={topData} />
        </div>
        <div className="col-md-4">
          <h2>Gráfico de Pie</h2>
          <PieChart />
        </div>
      </div>
      <hr className="my-4" />
      <h2>Procesos</h2>
      <ProcessTable columns={columns} data={processData} />
      <hr className="my-4" />
      <h2>Solicitudes</h2>
      <ProcessTable columns={requestsColumns} data={requestsData} />
    </div>
  );
};

export default App;
