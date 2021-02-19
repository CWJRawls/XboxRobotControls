inlets = 1;
outlets = 1;

var states = new Array();

function setState(channel, value) {
	for(var i = 0; i < states.length; i += 1) {
		if(states[i].channel == channel) {
			states[i].value = value;
			return;
		}
	}
	
	var state = {channel: channel, value: value};
	states.push(state);
}

function clear() {
	states = new Array();
}

function sendState() {

	if(states.length > 0) {
		outlet(0, "rows", states.length);
	}

	for(var i = 0; i < states.length; i += 1) {
		outlet(0, "set", 0, i, states[i].channel);
		outlet(0, "set", 1, i, states[i].value);
	}
}