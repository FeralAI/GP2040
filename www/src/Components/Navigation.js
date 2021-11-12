import React from 'react';
import { Container, Nav, NavDropdown, Navbar } from 'react-bootstrap';
import { NavLink } from "react-router-dom";
import './Navigation.scss';

function Navigation(props) {
	return (
		<Navbar bg="dark" variant="dark" expand="md">
			<Navbar.Brand href="/">
				<img src="images/logo.png" className="title-logo" alt="logo" />{' '}GP2040
			</Navbar.Brand>
			<Navbar.Toggle aria-controls="basic-navbar-nav" />
			<Navbar.Collapse id="basic-navbar-nav">
				<Nav>
					<Nav.Item>
						<NavLink exact={true} className="nav-link" activeClassName="active" to="/">Home</NavLink>
					</Nav.Item>
					<Nav.Item>
						<NavLink exact={true} className="nav-link" activeClassName="active" to="/settings">Settings</NavLink>
					</Nav.Item>
					<NavDropdown title="Configuration">
						<NavDropdown.Item>
							<NavLink exact={true} className="dropdown-item" activeClassName="active" to="/pin-mapping">Pin Mapping</NavLink>
						</NavDropdown.Item>
						<NavDropdown.Item>
							<NavLink exact={true} className="dropdown-item" activeClassName="active" to="/led-config">LED Configuration</NavLink>
						</NavDropdown.Item>
					</NavDropdown>
					<NavDropdown title="Links">
						<NavDropdown.Item>
							<a className="dropdown-item" target="_blank" rel="noreferrer" href="https://feralai.github.io/GP2040/">Documentation</a>
						</NavDropdown.Item>
						<NavDropdown.Item>
							<a className="dropdown-item" target="_blank" rel="noreferrer" href="https://github.com/FeralAI/GP2040/">Github</a>
						</NavDropdown.Item>
					</NavDropdown>
					<NavDropdown title="DANGER ZONE" className="btn-danger danger-zone">
						<NavDropdown.Item>
							<NavLink exact={true} className="dropdown-item" activeClassName="active" to="/reset-settings">Reset Settings</NavLink>
						</NavDropdown.Item>
					</NavDropdown>
				</Nav>
			</Navbar.Collapse>
		</Navbar>
	);
}

export default Navigation;
