const ctx = document.getElementById("chart").getContext("2d");
const chart = new Chart(ctx, {
  type: "line",
  data: {
    labels: [],
    datasets: [
      {
        label: "Nível de Gás",
        data: [],
        backgroundColor: "rgba(255, 99, 132, 0.2)",
        borderColor: "rgba(255, 99, 132, 1)",
        borderWidth: 1,
      },
    ],
  },
  options: {
    scales: {
      y: {
        beginAtZero: true,
      },
    },
  },
});
var dbPathHum = '/statusSensor';
var dbRefHum = firebase.database().ref().child(dbPathHum);
// Escuta as atualizações do sensor MQ2 no Firebase Realtime Database
dbRefHum.on('child_added', (snapshot) => {
    const data = snapshot.val();
    if (data !== null) {
      chart.data.labels.push(data.timestamp);
      chart.data.datasets[0].data.push(data.value);
      chart.update();
    } else {
      console.log('No data found at /statusSensor');
    }
  });

  document.getElementById("reset-button").addEventListener("click", function() {
    if (confirm("Tem certeza de que deseja limpar o gráfico?")) {
      chart.data.labels = [];
      chart.data.datasets[0].data = [];
      chart.update();
    }
  });