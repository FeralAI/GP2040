import React, { useEffect, useState } from 'react';
import axios from 'axios';
import { filter, orderBy } from 'lodash';

export default function HomePage() {
	const [currentVersion, setCurrentVersion] = useState(process.env.REACT_APP_CURRENT_VERSION);
	const [latestVersion, setLatestVersion] = useState('');

	useEffect(() => {
		axios.get('https://api.github.com/repos/FeralAI/GP2040/releases')
			.then((response) => {
				const sortedData = orderBy(response.data, 'published_at', 'desc');
				setLatestVersion(sortedData[0].name);
			})
			.catch(console.error);
	}, [setLatestVersion]);

	return (
		<div>
			<h1>Welcome to the GP2040 web configurator!</h1>
			<p>Please select a menu option to proceed.</p>
			<div>
				<h5>Version</h5>
				<div>Current Version: { currentVersion }</div>
				<div>Latest Version: { latestVersion }</div>
				{(currentVersion === latestVersion) ?
					<div>
						<a
							target="_blank"
							rel="noreferrer"
							href={`https://github.com/FeralAI/GP2040/releases/${latestVersion}`}
						>
							Get latest version
						</a>
					</div>
				: null}
			</div>
		</div>
	);
}
