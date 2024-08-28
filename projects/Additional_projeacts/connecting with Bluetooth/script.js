// script.js

document.addEventListener("DOMContentLoaded", function () {
    const connectBtn = document.getElementById("connectBtn");
    const ledToggle = document.getElementById("ledToggle");
    const status = document.getElementById("status");

    let device = null;
    let characteristic = null;

    async function connectBluetooth() {
        try {
            console.log('Requesting Bluetooth device...');
            // Request Bluetooth device
            device = await navigator.bluetooth.requestDevice({
                filters: [{ services: ['0000ffe0-0000-1000-8000-00805f9b34fb'] }]
            });

            console.log('Connecting to GATT server...');
            // Connect to the GATT server
            const server = await device.gatt.connect();
            console.log('Connected to GATT server');

            console.log('Getting primary service...');
            const service = await server.getPrimaryService('0000ffe0-0000-1000-8000-00805f9b34fb');
            console.log('Primary service found');

            console.log('Getting characteristic...');
            characteristic = await service.getCharacteristic('0000ffe1-0000-1000-8000-00805f9b34fb');
            console.log('Characteristic found');
            
            // Update status
            status.textContent = "Connected to Bluetooth device";
            status.style.color = "#38ef7d";
        } catch (error) {
            console.error('Error connecting to Bluetooth device:', error);
            status.textContent = "Connection failed";
            status.style.color = "#ff416c";
        }
    }

    async function sendBluetoothCommand(command) {
        if (characteristic) {
            try {
                await characteristic.writeValue(new TextEncoder().encode(command));
                console.log('Sent command:', command);
            } catch (error) {
                console.error('Error sending command:', error);
            }
        } else {
            console.log('Not connected to a Bluetooth device');
        }
    }

    connectBtn.addEventListener("click", function () {
        connectBluetooth();
    });

    ledToggle.addEventListener("change", function () {
        if (ledToggle.checked) {
            status.textContent = "LED is ON";
            status.style.color = "#38ef7d";
            sendBluetoothCommand("1");
        } else {
            status.textContent = "LED is OFF";
            status.style.color = "#ff416c";
            sendBluetoothCommand("0");
        }
    });
});
