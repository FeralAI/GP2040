import React from 'react';
import axios from 'axios';

class PinMappingPage extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			DpadUp:    -1, DpadDown:  -1, DpadLeft:  -1, DpadRight: -1,
			ButtonB1:  -1, ButtonB2:  -1, ButtonB3:  -1, ButtonB4:  -1,
			ButtonL1:  -1, ButtonR1:  -1, ButtonL2:  -1, ButtonR2:  -1,
			ButtonL3:  -1, ButtonR3:  -1, ButtonS1:  -1, ButtonS2:  -1,
			ButtonA1:  -1, ButtonA2:  -1, ButtonA3:  -1,
		};
	}

	componentDidMount() {
		axios.get('http://192.168.7.1/cgi/action?method=getPinMapping')
			.then((response) => {
				console.log(response);
			})
			.catch((error) => {
				console.error(error);
			});
	}

	render() {
		return (
			<div>
				<h1>Pin Mapping</h1>
			</div>
		);
	}
}

export default PinMappingPage;
