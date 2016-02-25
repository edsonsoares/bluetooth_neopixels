var NEO_SERVICE = 'AB10';
var SWITCH_CHARACTERISTIC = 'AB11';
var RED_CHARACTERISTIC = 'AB12';
var GREEN_CHARACTERISTIC = 'AB13';
var BLUE_CHARACTERISTIC = 'AB14';


window.onerror = function(e, fileName, lineNumber){
    alert(fileName + 
        'Line:' +  lineNumber,
        'Error:' + e.message);
};

var app = {
    initialize: function() {
        this.bindEvents();
        this.showMainPage();
    },
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
        document.addEventListener('backbutton', this.onBackButton, false);
        deviceList.addEventListener('click', this.connect, false);
        refreshButton.addEventListener('click', this.refreshDeviceList, false);
        onButton.addEventListener('click', this.switchOn,false);
        //offButton.addEventListener('click', this.switchOff, false);
        red.addEventListener('change', this.setRed, false);
        green.addEventListener('change', this.setGreen, false);
        blue.addEventListener('change', this.setBlue, false);
        disconnectButton.addEventListener('click', this.disconnect, false);

    },
    onDeviceReady: function() {
        FastClick.attach(document.body); // https://github.com/ftlabs/fastclick
        app.refreshDeviceList();
    },
    refreshDeviceList: function() {
        deviceList.innerHTML = ''; // empty the list
        ble.scan([NEO_SERVICE], 5, app.onDiscoverDevice, app.onError);
    },
    onDiscoverDevice: function(device) {
        var listItem = document.createElement('li');
        listItem.innerHTML = device.name + '<br/>' +
            device.id + '<br/>' +
            'RSSI: ' + device.rssi;
        listItem.dataset.deviceId = device.id;
        deviceList.appendChild(listItem);
    },
    connect: function(e) {
        var deviceId = e.target.dataset.deviceId;
        ble.connect(deviceId, app.onConnect, app.onError);
    },

    onConnect: function(peripheral) {
        app.peripheral = peripheral;
        app.showDetailPage();
    },


    disconnect: function(e) {
        if (app.peripheral && app.peripheral.id) {
            ble.disconnect(app.peripheral.id, app.showMainPage, app.onError);
        }
    },

    switchOn: function(){
        app.setSwitchValue(1);
    },



    setRed: function(){
        var data = new Uint8Array(1);
        data[0] = red.value;

        var success = function() {
            console.log('Set red to ' + data[0]);
        };

        if (app.peripheral && app.peripheral.id){
            ble.write(
                app.peripheral.id,
                NEO_SERVICE,
                RED_CHARACTERISTIC,
                data.buffer,
                success,
                app.onError
                );
        }
    },


    setGreen: function(){
            var data = new Uint8Array(1);
            data[0] = green.value;

            var success = function() {
                console.log('Set red to ' + data[0]);
            };

            if (app.peripheral && app.peripheral.id){
                ble.write(
                    app.peripheral.id,
                    NEO_SERVICE,
                    GREEN_CHARACTERISTIC,
                    data.buffer,
                    success,
                    app.onError
                    );
            }
        },

    setBlue: function(){
        var data = new Uint8Array(1);
        data[0] = blue.value;

        var success = function() {
            console.log('Set red to ' + data[0]);
        };

        if (app.peripheral && app.peripheral.id){
            ble.write(
                app.peripheral.id,
                NEO_SERVICE,
                BLUE_CHARACTERISTIC,
                data.buffer,
                success,
                app.onError
                );
        }
    },



    setSwitchValue: function(value){
        var success = function(){
            console.log('Set swtich to' + value);
        };

        if (app.peripheral && app.peripheral.id){
            var data = new Uint8Array(1);
            data[0] = value;
            ble.write(
                app.peripheral.id,
                NEO_SERVICE,
                SWITCH_CHARACTERISTIC,
                data.buffer,
                success,
                app.onError
                );
        }
    },

    showMainPage: function() {
        mainPage.hidden = false;
        detailPage.hidden = true;
    },
    showDetailPage: function() {
        mainPage.hidden = true;
        detailPage.hidden = false;
    },
    onBackButton: function() {
        if (mainPage.hidden) {
            app.disconnect();
        } else {
            navigator.app.exitApp();
        }
    },
    onError: function(reason) {
        navigator.notification.alert(reason, app.showMainPage, 'Error');
    }
};

app.initialize();