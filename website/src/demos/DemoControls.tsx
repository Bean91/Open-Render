import React, {type ChangeEvent} from "react";
import DemoCanvas from "./DemoCanvas.tsx";
import "../style.css";
import { useLocalStorage } from "../hooks/useLocalStorage.ts";

interface color {
	r: number;
	g: number;
	b: number;
	a: number;
}

interface request {
	type: string;
	name: string;
	roll: number;
	pitch: number;
	yaw: number;
	color: color;
	inputPoints: number[];
}

const DemoControls: React.FC = () => {
	const [requestData, setRequestData] = useLocalStorage<request[]>("requestData", []);
	const [roll, setRoll] = useLocalStorage<number>("roll", 0);
	const [pitch, setPitch] = useLocalStorage<number>("pitch", 0);
	const [yaw, setYaw] = useLocalStorage<number>("yaw", 0);
	const [x, setX] = useLocalStorage<number>("x", 0);
	const [y, setY] = useLocalStorage<number>("y", 0);
	const [z, setZ] = useLocalStorage<number>("z", 0);

	function changeRoll(event: ChangeEvent<HTMLInputElement>) {
		setRoll(parseInt(event.target.value));
	}
	function changePitch(event: ChangeEvent<HTMLInputElement>) {
		setPitch(parseInt(event.target.value));
	}
	function changeYaw(event: ChangeEvent<HTMLInputElement>) {
		setYaw(parseInt(event.target.value));
	}

	function changeX(event: ChangeEvent<HTMLInputElement>) {
		setX(parseInt(event.target.value));
	}
	function changeY(event: ChangeEvent<HTMLInputElement>) {
		setY(parseInt(event.target.value));
	}
	function changeZ(event: ChangeEvent<HTMLInputElement>) {
		setZ(parseInt(event.target.value));
	}

	function changeRequestType(index: number, e: ChangeEvent<HTMLSelectElement>) {
		const updatedValues = requestData.map((item, i) => {
			if (i === index) {
				return { ...item, type: e.target.value };
			}
			return item;
		});
		setRequestData(updatedValues);
	}
	function changeRequestName(index: number, e: ChangeEvent<HTMLInputElement>) {
		const updatedValues = requestData.map((item, i) => {
			if (i === index) {
				return { ...item, name: e.target.value };
			}
			return item;
		});
		setRequestData(updatedValues);
	}
	function changeRequestPoint(index: number, pointIndex: number, e: ChangeEvent<HTMLInputElement>) {
		const updatedValues = requestData.map((item, i) => {
			if (i === index) {
				const newInputPoints = item.inputPoints.map((point, pi) => {
					if (pi === pointIndex) {
						return parseInt(e.target.value);
					}
					return point;
				});
				return { ...item, inputPoints: newInputPoints };
			}
			return item;
		});
		setRequestData(updatedValues);
	}
	function changeRequestRoll(index: number, e: ChangeEvent<HTMLInputElement>) {
		const updatedValues = requestData.map((item, i) =>
			i === index ? { ...item, roll: parseInt(e.target.value) } : item
		);
		setRequestData(updatedValues);
	}
	function changeRequestPitch(index: number, e: ChangeEvent<HTMLInputElement>) {
		const updatedValues = requestData.map((item, i) =>
			i === index ? { ...item, pitch: parseInt(e.target.value) } : item
		);
		setRequestData(updatedValues);
	}
	function changeRequestYaw(index: number, e: ChangeEvent<HTMLInputElement>) {
		const updatedValues = requestData.map((item, i) =>
			i === index ? { ...item, yaw: parseInt(e.target.value) } : item
		);
		setRequestData(updatedValues);
	}

	function changeRequestR(index: number, e: ChangeEvent<HTMLInputElement>) {
		const updatedValues = requestData.map((item, i) =>
			i === index ? { ...item, color: {r: parseInt(e.target.value), g: item.color.g, b: item.color.b, a: item.color.a} } : item
		);
		setRequestData(updatedValues);
	}

	function changeRequestG(index: number, e: ChangeEvent<HTMLInputElement>) {
		const updatedValues = requestData.map((item, i) =>
			i === index ? { ...item, color: {r: item.color.r, g: parseInt(e.target.value), b: item.color.b, a: item.color.a} } : item
		);
		setRequestData(updatedValues);
	}

	function changeRequestB(index: number, e: ChangeEvent<HTMLInputElement>) {
		const updatedValues = requestData.map((item, i) =>
			i === index ? { ...item, color: {r: item.color.r, g: item.color.g, b: parseInt(e.target.value), a: item.color.a} } : item
		);
		setRequestData(updatedValues);
	}

	function changeRequestA(index: number, e: ChangeEvent<HTMLInputElement>) {
		const updatedValues = requestData.map((item, i) =>
			i === index ? { ...item, color: {r: item.color.r, g: item.color.g, b: item.color.b, a: parseInt(e.target.value)} } : item
		);
		setRequestData(updatedValues);
	}

	function removeObject(index: number) {
		setRequestData(data => data.filter((_, i) => i !== index));
	}
	function newObject() {
		const newItem = {type: "line", name: "", roll: 0, pitch: 0, yaw: 0, inputPoints: [0, 0, 0, 0, 0, 0, 0, 0, 0], color: {r: 0, g: 0, b: 0, a: 0}};
		setRequestData(data => [...data, newItem]);
	}

	const inputItem = "w-full border border-gray-700 rounded-sm p-1";
	return (
		<div className={"flex items-center justify-items h-[calc(100vh-4rem)] mt-16 bg-gray-500"}>
			<DemoCanvas requestData={requestData} roll={roll} pitch={pitch} yaw={yaw} x={x} y={y} z={z} />
			<div id={"controls"} className={"max-h-3/4 scrollbar-thin overflow-x-hidden overflow-y-scroll w-1/4 flex flex-col h-child justify-between items-center m-4 p-2 bg-gray-300 rounded-md"}>
				<div className={"grid grid-cols-3 gap-2 block w-full border p-1 m-1 rounded-sm block"}>
					<h1 className={"col-span-3 text-center"}>View Controls</h1>
					<h3 className={"text-center"}>Roll</h3>
					<h3 className={"text-center"}>Pitch</h3>
					<h3 className={"text-center"}>Yaw</h3>
					<input className={inputItem} type="number" step={20} min={0} max={360} maxLength={3} value={roll} onChange={changeRoll} />
					<input className={inputItem} type="number" step={20} min={0} max={360} maxLength={3} value={pitch} onChange={changePitch} />
					<input className={inputItem} type="number" step={20} min={0} max={360} maxLength={3} value={yaw} onChange={changeYaw} />
					<h3 className={"text-center"}>X</h3>
					<h3 className={"text-center"}>Y</h3>
					<h3 className={"text-center"}>Z</h3>
					<input className={inputItem} type="number" step={20} value={x} onChange={changeX} />
					<input className={inputItem} type="number" step={20} value={y} onChange={changeY} />
					<input className={inputItem} type="number" step={20} value={z} onChange={changeZ} />
				</div>
				{requestData.map((val: request, index: number) => {
					return (
						<div className={inputItem + " grid gap-2 grid-cols-2 m-1"} key={index}>
							<select className={inputItem + " col-span-1"} value={val.type} onChange={(e) => changeRequestType(index, e)}>
								<option value={"line"}>Line</option>
								<option value={"triangle"}>Triangle</option>
								<option value={"recPrOut"}>Rectangle Outline</option>
								<option value={"recPrFill"}>Rectangle</option>
								<option value={"circle"}>Circle</option>
								<option value={"sphere"}>Sphere</option>
							</select>
							<input className={inputItem + " col-span-1"} value={val.name} placeholder={"Name..."} onChange={(e) => changeRequestName(index, e)} />
							<div className={inputItem + " grid gap-2 grid-cols-3 col-span-2"}>
								<h3 className={"col-span-3 text-center"}>Points List</h3>
									{(val.type === "line") ? <> <input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[0]} onChange={(e) => changeRequestPoint(index, 0, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[1]} onChange={(e) => changeRequestPoint(index, 1, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[2]} onChange={(e) => changeRequestPoint(index, 2, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[3]} onChange={(e) => changeRequestPoint(index, 3, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[4]} onChange={(e) => changeRequestPoint(index, 4, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[5]} onChange={(e) => changeRequestPoint(index, 5, e)} step={20} /> </> :
									(val.type === "recPrOut" || val.type === "recPrFill") ? <> <input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[0]} onChange={(e) => changeRequestPoint(index, 0, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[1]} onChange={(e) => changeRequestPoint(index, 1, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[2]} onChange={(e) => changeRequestPoint(index, 2, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[3]} min={0} onChange={(e) => changeRequestPoint(index, 3, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[4]} min={0} onChange={(e) => changeRequestPoint(index, 4, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[5]} min={0} onChange={(e) => changeRequestPoint(index, 5, e)} step={20} /> </> :
									(val.type === "triangle") ? <> <input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[0]} onChange={(e) => changeRequestPoint(index, 0, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[1]} onChange={(e) => changeRequestPoint(index, 1, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[2]} onChange={(e) => changeRequestPoint(index, 2, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[3]} onChange={(e) => changeRequestPoint(index, 3, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[4]} onChange={(e) => changeRequestPoint(index, 4, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[5]} onChange={(e) => changeRequestPoint(index, 5, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[6]} onChange={(e) => changeRequestPoint(index, 6, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[7]} onChange={(e) => changeRequestPoint(index, 7, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[8]} onChange={(e) => changeRequestPoint(index, 8, e)} step={20} /> </> :
									(val.type === "circle" || val.type === "sphere") ? <> <input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[0]} onChange={(e) => changeRequestPoint(index, 0, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[1]} onChange={(e) => changeRequestPoint(index, 1, e)} step={20} />
										<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[2]} onChange={(e) => changeRequestPoint(index, 2, e)} step={20} />
										<input className={inputItem + " col-span-3"} type={"number"} value={val.inputPoints[3]} onChange={(e) => changeRequestPoint(index, 3, e)} step={20} /> </> : <></>}
							</div>
							{val.type !== "line" && val.type !== "sphere" && val.type !== "triangle" ?
								<div className={inputItem + " grid gap-2 grid-cols-3 col-span-2"}>
									<h3 className={"col-span-3 text-center"}>Rotations List</h3>
									<input className={inputItem + " col-span-1"} type={"number"} min={0} max={360} value={val.roll} onChange={(e) => changeRequestRoll(index, e)} step={20} />
									<input className={inputItem + " col-span-1"} type={"number"} min={0} max={360} value={val.pitch} onChange={(e) => changeRequestPitch(index, e)} step={20} />
									<input className={inputItem + " col-span-1"} type={"number"} min={0} max={360} value={val.yaw} onChange={(e) => changeRequestYaw(index, e)} step={20} />
								</div>
							: null}
							<div className={inputItem + " grid gap-2 grid-cols-4 col-span-2"}>
								<h3 className={"col-span-4 text-center"}>Colors (RGBA)</h3>
								<input className={inputItem + " col-span-1"} type={"number"} min={0} max={255} value={val.color.r} onChange={(e) => changeRequestR(index, e)} />
								<input className={inputItem + " col-span-1"} type={"number"} min={0} max={255} value={val.color.g} onChange={(e) => changeRequestG(index, e)} />
								<input className={inputItem + " col-span-1"} type={"number"} min={0} max={255} value={val.color.b} onChange={(e) => changeRequestB(index, e)} />
								<input className={inputItem + " col-span-1"} type={"number"} min={0} max={255} value={val.color.a} onChange={(e) => changeRequestA(index, e)} />
							</div>
							<button className={inputItem + " col-span-2 text-center cursor-pointer"} onClick={() => removeObject(index)}>Delete Object</button>
						</div>)
				})}
				<button className={inputItem + " col-span-3 text-center cursor-pointer m-1"} onClick={newObject}>Add New Object</button>
			</div>
		</div>
	);
}

export default DemoControls;