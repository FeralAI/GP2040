import React from 'react';
import { NavLink } from "react-router-dom";

function Navbar(props) {
	return (
		<nav className="navbar navbar-expand-md navbar-dark fixed-top bg-dark">
			<div className="container-fluid">
				<NavLink className="navbar-brand" to="/">GP2040 Configurator</NavLink>
				<button className="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarCollapse" aria-controls="navbarCollapse" aria-expanded="false" aria-label="Toggle navigation">
					<span className="navbar-toggler-icon"></span>
				</button>
				<div className="collapse navbar-collapse" id="navbarCollapse">
					<ul className="navbar-nav me-auto mb-2 mb-md-0">
						<li className="nav-item">
							<NavLink exact={true} className="nav-link" activeClassName="active" to="/">Home</NavLink >
						</li>
						<li className="nav-item">
							<NavLink exact={true} className="nav-link" activeClassName="active" to="/pin-mapping">Pin Mapping</NavLink >
						</li>
						<li className="nav-item">
							<NavLink exact={true} className="nav-link disabled" activeClassName="active" to="/path/to/nowhere">Disabled</NavLink >
						</li>
					</ul>
				</div>
			</div>
		</nav>
	);
}

export default Navbar;
