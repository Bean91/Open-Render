import React, { useState } from "react";
import "../style.css";
import DemoInput from "./DemoInput.tsx";

interface request {
	type: string;
	name: string;
	roll: number;
	pitch: number;
	yaw: number;
	inputPoints: number[];
}

const DemoCanvas: React.FC = () => {
	const [roll, setRoll] = useState<number>(0);
	const [pitch, setPitch] = useState<number>(0);
	const [yaw, setYaw] = useState<number>(0);
	const [x, setX] = useState<number>(0);
	const [y, setY] = useState<number>(0);
	const [z, setZ] = useState<number>(0);

	const [requestData, setRequestData] = useState<request[]>([]);
	return (<div id={"controls"} className={"w-1/4 ml-auto h-16 m-4 p-2 bg-gray-300 rounded-md"}>
		{requestData.map((val: request, index: number) => <DemoInput key={index} roll={val.roll} pitch={val.pitch} yaw={val.yaw} name={val.name} type={val.type} inputPoints={val.inputPoints} />)}
	</div>);
}

export default DemoCanvas;