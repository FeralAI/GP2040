import React, { useContext } from 'react';
import { Nav, NavDropdown, Navbar } from 'react-bootstrap';
import { NavLink } from "react-router-dom";
import { AppContext } from '../Contexts/AppContext';
import FormSelect from './FormSelect';
import { saveButtonLabels } from '../Services/Storage';
import BUTTONS from '../Data/Buttons.json';
import './Navigation.scss';

const Navigation = (props) => {
	const { buttonLabels, setButtonLabels } = useContext(AppContext);

	const updateButtonLabels = (e) => {
		saveButtonLabels(e.target.value);
		setButtonLabels(e.target.value);
	};

	return (
		<Navbar bg="dark" variant="dark" expand="md" fixed="top">
			<Navbar.Brand href="/">
				<img src="images/logo.png" className="title-logo" alt="logo" />{' '}GP2040
			</Navbar.Brand>
			<Navbar.Collapse id="basic-navbar-nav">
				<Nav as="ul">
					<Nav.Item as="li">
						<NavLink exact={true} className="nav-link" activeClassName="active" to="/">Home</NavLink>
					</Nav.Item>
					<Nav.Item as="li">
						<NavLink exact={true} className="nav-link" activeClassName="active" to="/settings">Settings</NavLink>
					</Nav.Item>
					<NavDropdown as="li" title="Configuration">
						<NavLink exact={true} className="dropdown-item" activeClassName="active" to="/pin-mapping">Pin Mapping</NavLink>
						<NavLink exact={true} className="dropdown-item" activeClassName="active" to="/led-config">LED Configuration</NavLink>
					</NavDropdown>
					<NavDropdown as="li" title="Links">
						<a className="dropdown-item" target="_blank" rel="noreferrer" href="https://feralai.github.io/GP2040/">Documentation</a>
						<a className="dropdown-item" target="_blank" rel="noreferrer" href="https://github.com/FeralAI/GP2040/">Github</a>
					</NavDropdown>
					<NavDropdown as="li" title="DANGER ZONE" className="btn-danger danger-zone">
						<NavLink exact={true} className="dropdown-item" activeClassName="active" to="/reset-settings">Reset Settings</NavLink>
					</NavDropdown>
				</Nav>
			</Navbar.Collapse>
			<div className="navbar-tools">
				<FormSelect
					name="buttonLabels"
					className="form-select-sm"
					value={buttonLabels}
					onChange={updateButtonLabels}
				>
					{Object.keys(BUTTONS).map((b, i) =>
						<option key={`button-label-option-${i}`} value={BUTTONS[b].value}>{BUTTONS[b].label}</option>
						)}
				</FormSelect>
				<Navbar.Toggle aria-controls="basic-navbar-nav" />
			</div>
		</Navbar>
	);
};

export default Navigation;
