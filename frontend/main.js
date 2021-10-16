// var percentage = document.getElementsByClassName('percentage')[0]
// percentage.style['transform'] = 'rotate(170deg)'

const updateTime = 5000
let counter = 1

let main = setInterval(requestAPI, 5000)
function requestAPI(){
	let url = 'http://localhost:8000/record-last/1/';
	let response;
	fetch(url)
	    .then(res => res.json())
	    .then(data => response = data)
	    .then(() => update(response))
}


function update(response){
	let phGauge = document.getElementById('ph').children;
	let tempGauge = document.getElementById('temp').children;
	let humidityGauge = document.getElementById('humidity').children;

	tempPercentage = tempGauge[0], tempValue = tempGauge[2];
	phPercentage = phGauge[0], phValue = phGauge[2];
	humidityPercentage = humidityGauge[0], humidityValue = humidityGauge[2];

	tempValue.innerHTML = parseFloat(response['temperature']).toFixed(1) + ' ºC';
	phValue.innerHTML = response['ph'];
	humidityValue.innerHTML = response['humidity']+ "%";

	moveGauge(tempPercentage, response['temperature'] / 40)
	moveGauge(phPercentage, response['ph'] / 14)
	moveGauge(humidityPercentage, response['humidity'] / 100)
	counter += 1
}


function moveGauge(actualPercent, toMove){
	let actualDeg = Math.floor(strToFloat(actualPercent.style['transform']));
	let newDeg = Math.floor(toMove * 180);

	console.log(counter, actualDeg, newDeg)

	let i = 0	
	if (i == 0){
		i = 1
		let change = setInterval(frame, 5);
		function frame(){
			if (actualDeg == newDeg){
				i = 0;
				clearInterval(change);
			}
			else if (actualDeg < newDeg){
				actualDeg += 1
				actualPercent.style['transform'] = 'rotate(' + String(actualDeg) + 'deg)'
			}
			else if (actualDeg > newDeg){
				actualDeg -= 1
				actualPercent.style['transform'] = 'rotate(' + String(actualDeg) + 'deg)'
			}
		}
	}
	actualPercent.style['transform'] = 'rotate(' + String(newDeg) + 'deg)';
}

function strToFloat(degrees){
	return degrees.slice(7, degrees.indexOf('d'))
}