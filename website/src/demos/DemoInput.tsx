import React from "react";
import "../style.css";

interface input {
	type: string;
	name: string;
	roll: number;
	pitch: number;
	yaw: number;
	inputPoints: number[];
}

const DemoInput: React.FC<input> = (input) => {
	return (<div>
		<select value={input.type}>
			<option value={}>Line</option>
		</select>
	</div>);
}

export default DemoInput