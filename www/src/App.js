import React from 'react';
import { BrowserRouter as Router, Route, Switch } from "react-router-dom";
import './App.scss';

import Navbar from './Components/Navbar'
import HomePage from './Pages/HomePage'
import PinMappingPage from "./Pages/PinMapping";
import ResetSettingsPage from './Pages/ResetSettingsPage';
import SettingsPage from './Pages/SettingsPage';
import LEDConfigPage from './Pages/LEDConfigPage';

function App() {
	return (
		<Router>
			<Navbar />
			<div className="container-fluid body-content">
				<Switch>
					<Route exact path="/">
						<HomePage />
					</Route>
					<Route path="/settings">
						<SettingsPage />
					</Route>
					<Route path="/pin-mapping">
						<PinMappingPage />
					</Route>
					<Route path="/reset-settings">
						<ResetSettingsPage />
					</Route>
					<Route path="/led-config">
						<LEDConfigPage />
					</Route>
				</Switch>
			</div>
		</Router>
	);
}

export default App;
