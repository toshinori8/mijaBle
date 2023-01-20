const sensorDataDay = {
    labels: ['00:00', '01:00', '02:00', '03:00', '04:00', '05:00', '06:00', '07:00', '08:00', '09:00', '10:00', '11:00', '12:00', '13:00', '14:00', '15:00', '16:00', '17:00', '18:00', '19:00', '19:30'],
    datasets: [
        {
            label: 'Temperatura aktualna',
            data: [20, 21, 22, 21, 20, 18, 17, 16, 18, 20, 22, 21, 23, 25, 26, 27, 28, 29, 30, 31, 32],
            backgroundColor: 'rgba(255, 99, 132, 0.2)',
            borderColor: 'rgba(255, 99, 132, 1)',
        },
        {
            label: 'Temperatura zadana',
            data: [21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21],
            backgroundColor: 'rgba(54, 162, 235, 0.2)',
            borderColor: 'rgba(54, 162, 235, 1)',
        },
        {
            label: 'Temperatura na zewnątrz',
            data: [10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50],
            backgroundColor: 'rgba(153, 102, 255, 0.2)',
            borderColor: 'rgba(153, 102, 255, 1)',
        },
        {
            label: 'Włączenie ogrzewania',
            data: [false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, false],
            backgroundColor: 'rgba(255, 206, 86, 0.2)',
            borderColor: 'rgba(255, 206, 86, 1)',
        }
    ]
}


const sensorDataWeek = {
    labels: ['Poniedziałek', 'Wtorek', 'Środa', 'Czwartek', 'Piątek', 'Sobota', 'Niedziela'],
    datasets: [
        {
            label: 'Temperatura aktualna',
            data: [20, 21, 22, 23, 24, 25, 26],
            backgroundColor: 'rgba(255, 99, 132, 0.2)',
            borderColor: 'rgba(255, 99, 132, 1)',
        },
        {
            label: 'Temperatura zadana',
            data: [22, 22, 22, 22, 22, 22, 22],
            backgroundColor: 'rgba(54, 162, 235, 0.2)',
            borderColor: 'rgba(54, 162, 235, 1)',
        },
        {
            label: 'Temperatura na zewnątrz',
            data: [10, 12, 14, 16, 18, 20, 22],
            backgroundColor: 'rgba(153, 102, 255, 0.2)',
            borderColor: 'rgba(153, 102, 255, 1)',
        },
        {
            label: 'Włączenie ogrzewania',
            data: [true, true, true, true, true, false, false],
            backgroundColor: 'rgba(255, 206, 86, 0.2)',
            borderColor: 'rgba(255, 206, 86, 1)',
        }
    ]
}




let genMonthData = function (month) {

  const months = ["styczeń", "luty", "marzec", "kwiecień", "maj", "czerwiec", "lipiec", "sierpień", "wrzesień", "październik", "listopad", "grudzień"];


  let daysInMonth = new Date(2020, month, 0).getDate();
  let labels = [];
  let data = [];
  for (let i = 1; i <= daysInMonth; i++) {
    labels.push(i);
    data.push(Math.floor(Math.random() * 10) + 20);
  }

  return {
    labels: labels,
    datasets: [
      {
        label: 'Temperatura aktualna',
        data: data,
        backgroundColor: 'rgba(255, 99, 132, 0.2)',
        borderColor: 'rgba(255, 99, 132, 1)',
      },
      {
        label: 'Temperatura zadana',
        data: data,
        backgroundColor: 'rgba(54, 162, 235, 0.2)',
        borderColor: 'rgba(54, 162, 235, 1)',
      },
      {
        label: 'Temperatura na zewnątrz',
        data: data,
        backgroundColor: 'rgba(153, 102, 255, 0.2)',
        borderColor: 'rgba(153, 102, 255, 1)',
      },
      {
        label: 'Włączenie ogrzewania',
        data: data,
        backgroundColor: 'rgba(255, 206, 86, 0.2)',
        borderColor: 'rgba(255, 206, 86, 1)',
      }
    ]
  }


//   let sensorDataMonth = {};
// // create JSON object with labels
//   sensorDataMonth.labels = [];
//   for (let i = 1; i <= 31; i++) {
//     sensorDataMonth.labels.push(i);

//   } 

// // create JSON object with datasets
//   sensorDataMonth.datasets = [];
//   let dataset = {};
//   dataset.label = 'Temperatura aktualna';
//   dataset.data = [];
//   for (let i = 1; i <= 31; i++) {
//     dataset.data.push(Math.floor(Math.random() * 10) + 20);
//   }
//   dataset.backgroundColor = 'rgba(255, 99, 132, 0.2)';
//   dataset.borderColor = 'rgba(255, 99, 132, 1)';
//   sensorDataMonth.datasets.push(dataset);

//   dataset = {};
//   dataset.label = 'Temperatura zadana';
//   dataset.data = [];
//   for (let i = 1; i <= 31; i++) {
//     dataset.data.push(21);
//   }
//   dataset.backgroundColor = 'rgba(54, 162, 235, 0.2)';
//   dataset.borderColor = 'rgba(54, 162, 235, 1)';
//   sensorDataMonth.datasets.push(dataset);





  
//   months.forEach((month) => {
//       sensorDataMonth[month] = {
//           "averageCurrentTemperature": getRandomTemperature(-12, 20),
//           "averageTargetTemperature": getRandomTemperature(-12, 20),
//           "averageOutsideTemperature": getRandomTemperature(-10, 30),
//           "averageHeatingState": getRandomHeatingState(),
//           "heatingOnTime": getRandomHeatingTime()
//       }
//   });
  
//   function getRandomTemperature(min, max) {
//       return Math.floor(Math.random() * (max - min + 1)) + min;
//   }
  
//   function getRandomHeatingState() {
//       return Math.random() < 0.5;
//   }
  
//   function getRandomHeatingTime() {
//       let hours = Math.floor(Math.random() * 24);
//       let minutes = Math.floor(Math.random() * 60);
//       return `${hours}:${minutes}-${hours + 1}:${minutes}`;
//   }
//   console.log(sensorDataMonth);
  
  return sensorDataMonth;
  
}



  // var data = {
  //   labels: [
  //     "Styczeń",
  //     "Luty",
  //     "Marzec",
  //     "Kwiecień",
  //     "Maj",
  //     "Czerwiec",
  //     "Lipiec",
  //     "Sierpień",
  //     "Wrzesień",
  //     "Październik",
  //     "Listopad",
  //     "Grudzień",
  //   ],
  //   datasets: [
  //     {
  //       label: "Temperatura aktualna",
  //       data: [65, 59, 80, 81, 56, 55, 40, 60, 70, 80, 90, 100],
  //       backgroundColor: "rgba(75,192,192,0.4)",
  //       borderColor: "rgba(75,192,192,1)",
  //       borderWidth: 1,
  //       type: "line",
  //     },
  //     {
  //       label: "Temperatura zadana",
  //       data: [70, 75, 80, 75, 70, 75, 80, 75, 70, 75, 80, 75],
  //       backgroundColor: "rgba(255, 99, 132, 0.4)",
  //       borderColor: "rgba(255, 99, 132, 1)",
  //       borderWidth: 1,
  //       type: "line",
  //     },
  //     {
  //       label: "Temperatura na zewnątrz",
  //       data: [25, 30, 35, 30, 25, 30, 35, 30, 25, 30, 35, 30],
  //       backgroundColor: "rgba(255, 255, 0, 0.4)",
  //       borderColor: "rgba(255, 255, 0, 1)",
  //       borderWidth: 1,
  //       type: "line",
  //     },
  //     {
  //       label: "Stan włączenia ogrzewania",
  //       data: [50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50],
  //       backgroundColor: "rgba(0, 0, 255, 0.4)",
  //       borderColor: "rgba(0, 0, 255, 1)",
  //       borderWidth: 1,
  //       type: "bar",
  //     },
  //   ],
  // };

  // var options = {
  //   scales: {
  //     y: {
  //       beginAtZero: true,
  //     },
  //   },
  // };

  // var myChart = new Chart(ctx, {
  //   type: "bar",
  //   data: data,
  //   options: options,
  // });

  // ...
// });
