import React from 'react';
import { BrowserRouter as Router, Route, Switch } from "react-router-dom";
import './App.css';
import Navbar from './Components/Navbar'
import HomePage from './Pages/HomePage'
import PinMappingPage from "./Pages/PinMapping";

function App() {
	return (
		<div className="App">
			<Router>
				<Navbar />
				<Switch>
					<Route exact path="/">
						<HomePage />
					</Route>
					<Route path="/pin-mapping">
						<PinMappingPage />
					</Route>
				</Switch>
			</Router>
		</div>
	);
}

export default App;
