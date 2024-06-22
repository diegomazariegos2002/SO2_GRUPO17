import React, { useEffect, useRef } from 'react';
import Chart from 'chart.js/auto';

const PieChart = ({ data }) => {
  const chartRef = useRef(null);

  useEffect(() => {
    if (chartRef && chartRef.current && data.length > 0) {
      const ctx = chartRef.current.getContext('2d');

      // Destruir gráfico existente si hay uno
      if (chartRef.current.chart) {
        chartRef.current.chart.destroy();
      }

      // Configurar nuevo gráfico de pastel
      const newChartInstance = new Chart(ctx, {
        type: 'pie',
        data: {
          labels: data.map(item => item.name),
          datasets: [{
            data: data.map(item => parseFloat(item.sizePor)), 
            backgroundColor: generateRandomColors(data.length),
          }]
        },
        options: {
          responsive: true,
          plugins: {
            legend: {
              position: 'top',
            },
            tooltip: {
              callbacks: {
                label: function(tooltipItem) {
                  return `${tooltipItem.label}: ${tooltipItem.raw.toFixed(2)}%`;
                }
              }
            }
          }
        }
      });

      chartRef.current.chart = newChartInstance;
    }
  }, [data]);

  // Función para generar colores aleatorios
  const generateRandomColors = (numColors) => {
    const colors = [];
    for (let i = 0; i < numColors; i++) {
      colors.push(`rgba(${Math.floor(Math.random() * 256)}, ${Math.floor(Math.random() * 256)}, ${Math.floor(Math.random() * 256)}, 0.6)`);
    }
    return colors;
  };

  return (
    <canvas ref={chartRef} id="myPieChart" width="400" height="400"></canvas>
  );
};

export default PieChart;
