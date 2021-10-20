import { Router, Route, Switch } from "react-router-dom";
import { createBrowserHistory } from 'history'
import './App.css';
import Navbar from './Components/Navbar'
import HomePage from './Home'

const browserHistory = createBrowserHistory();

function App() {
	return (
		<div className="App">
			<Navbar />
			<Router history={browserHistory}>
				<Switch>
					<Route exact path="/">
						<HomePage />
					</Route>
				</Switch>
			</Router>
		</div>
	);
}

export default App;
