require('dotenv').config()

const SerialPort = require('serialport')
const port = new SerialPort(process.env.SERIALPORT, {
	baudRate: 9600,
})

SerialPort.list((err, res) => {
	if (err) {
		throw err;
	}

	
})

function turnOn ()  {
	console.log('sending turn on signal')
	port.write('on\n', function(err) {
		if (err) {
			return console.log('Error on write: ', err.message)
		}
		console.log('message written')
	})
}

function turnOff ()  {
	console.log('sending turn off signal')
	port.write('off\n', function(err) {
		if (err) {
			return console.log('Error on write: ', err.message)
		}
		console.log('message written')
	})
}

var mqtt = require('mqtt')
var client  = mqtt.connect(process.env.MQTT_URL, {
	username: process.env.MQTT_USER,
	password: process.env.MQTT_PASSWORD
})

client.on('connect', function () {
	client.subscribe('printer', function (err) {
		if (err) {
			throw err;
		}
		client.on('message', function (topic, message) {
			if (topic != 'printer') {
				return;
			}

			if (message === 'on') {
				turnOn();
			}
			if (message === 'off') {
				turnOff();
			}
		})
	})
})

