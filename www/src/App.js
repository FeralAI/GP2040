import { BrowserRouter, Route, Switch } from "react-router-dom";
import { createBrowserHistory } from 'history'
import './App.css';
import Navbar from './Components/Navbar'
import HomePage from './Pages/HomePage'
import PinMappingPage from "./Pages/PinMapping";

const browserHistory = createBrowserHistory();

function App() {
	return (
		<div className="App">
			<Navbar />
			<BrowserRouter history={browserHistory}>
				<Switch>
					<Route exact path="/">
						<HomePage />
					</Route>
					<Route exact path="/pin-mapping">
						<PinMappingPage />
					</Route>
				</Switch>
			</BrowserRouter>
		</div>
	);
}

export default App;
