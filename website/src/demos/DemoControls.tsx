import React, {useEffect, useState} from "react";
import "../style.css";

interface request {
	type: string;
	name: string;
	roll: number;
	pitch: number;
	yaw: number;
	inputPoints: number[];
}

const DemoControls: React.FC = () => {
	const [requestData, setRequestData] = useState<request[]>(() => {
		const storedData = localStorage.getItem('requestData');
		return storedData ? JSON.parse(storedData) : [{type: "line", name: "test", roll: 0, pitch: 0, yaw: 0, inputPoints: [0, 0, 0, 0, 0, 0, 0, 0, 0]}];
	});
	const [roll, setRoll] = useState<number>(() => {
		const storedData = localStorage.getItem('roll');
		return storedData ? JSON.parse(storedData) : 0;
	});
	const [pitch, setPitch] = useState<number>(() => {
		const storedData = localStorage.getItem('pitch');
		return storedData ? JSON.parse(storedData) : 0;
	});
	const [yaw, setYaw] = useState<number>(() => {
		const storedData = localStorage.getItem('yaw');
		return storedData ? JSON.parse(storedData) : 0;
	});
	const [x, setX] = useState<number>(() => {
		const storedData = localStorage.getItem('x');
		return storedData ? JSON.parse(storedData) : 0;
	});
	const [y, setY] = useState<number>(() => {
		const storedData = localStorage.getItem('y');
		return storedData ? JSON.parse(storedData) : 0;
	});
	const [z, setZ] = useState<number>(() => {
		const storedData = localStorage.getItem('z');
		return storedData ? JSON.parse(storedData) : 0;
	});

	useEffect(() => {
		localStorage.setItem('requestData', JSON.stringify(requestData));
	}, [requestData]);
	useEffect(() => {
		localStorage.setItem('roll', JSON.stringify(roll));
	}, [roll]);
	useEffect(() => {
		localStorage.setItem('pitch', JSON.stringify(pitch));
	}, [pitch]);
	useEffect(() => {
		localStorage.setItem('yaw', JSON.stringify(yaw));
	}, [yaw]);
	useEffect(() => {
		localStorage.setItem('x', JSON.stringify(x));
	}, [x]);
	useEffect(() => {
		localStorage.setItem('y', JSON.stringify(y));
	}, [y]);
	useEffect(() => {
		localStorage.setItem('z', JSON.stringify(z));
	}, [z]);

	function changeRoll(event: Event) {
		setRoll(event.target.value);
	}
	function changePitch(event: Event) {
		setPitch(event.target.value);
	}
	function changeYaw(event: Event) {
		setYaw(event.target.value);
	}

	function changeX(event: Event) {
		setX(event.target.value);
	}
	function changeY(event: Event) {
		setY(event.target.value);
	}
	function changeZ(event: Event) {
		setZ(event.target.value);
	}

	function changeRequestType(index: number, e: Event) {
		const updatedValues = requestData.map((item, i) => {
			if (i === index) {
				return { ...item, type: e.target.value };
			}
			return item;
		});
		setRequestData(updatedValues);
	}

	function changeRequestName(index: number, e: Event) {
		const updatedValues = requestData.map((item, i) => {
			if (i === index) {
				return { ...item, name: e.target.value };
			}
			return item;
		});
		setRequestData(updatedValues);
	}

	function changeRequestPoint(index: number, pointIndex: number, e: Event) {
		const updatedValues = requestData.map((item, i) => {
			if (i === index) {
				const newInputPoints = item.inputPoints.map((point, pi) => {
					if (pi === pointIndex) {
						return e.target.value;
					}
					return point;
				});
				return { ...item, inputPoints: newInputPoints };
			}
			return item;
		});
		setRequestData(updatedValues);
	}

	function changeRequestRoll(index: number, e: Event) {
		const updatedValues = requestData.map((item, i) =>
			i === index ? { ...item, roll: e.target.value } : item
		);
		setRequestData(updatedValues);
	}

	function changeRequestPitch(index: number, e: Event) {
		const updatedValues = requestData.map((item, i) =>
			i === index ? { ...item, pitch: e.target.value } : item
		);
		setRequestData(updatedValues);
	}

	function changeRequestYaw(index: number, e: Event) {
		const updatedValues = requestData.map((item, i) =>
			i === index ? { ...item, yaw: e.target.value } : item
		);
		setRequestData(updatedValues);
	}

	function removeObject(index: number) {
		setRequestData(data => data.filter((item, i) => i !== index));
	}

	function newObject() {
		const newItem = {type: "line", name: "", roll: 0, pitch: 0, yaw: 0, inputPoints: [0, 0, 0, 0, 0, 0, 0, 0, 0]};
		setRequestData(data => [...data, newItem]);
	}

	const inputItem = "w-full border border-gray-700 rounded-sm p-1";
	return (<div id={"controls"} className={"max-h-3/4 scrollbar-thin overflow-x-hidden overflow-y-scroll w-1/4 flex flex-col h-child justify-between items-center m-4 p-2 bg-gray-300 rounded-md"}>
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
				<div className={inputItem + " grid gap-2 grid-cols-2 m-2"} key={index}>
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
						{(val.type === "line" || val.type === "recPrOut" || val.type === "recPrFill") ? <> <input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[0]} onChange={(e) => changeRequestPoint(index, 0, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[1]} onChange={(e) => changeRequestPoint(index, 1, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[2]} onChange={(e) => changeRequestPoint(index, 2, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[3]} onChange={(e) => changeRequestPoint(index, 3, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[4]} onChange={(e) => changeRequestPoint(index, 4, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[5]} onChange={(e) => changeRequestPoint(index, 5, e)} step={20}></input> </> :
							(val.type === "triangle") ? <> <input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[0]} onChange={(e) => changeRequestPoint(index, 0, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[1]} onChange={(e) => changeRequestPoint(index, 1, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[2]} onChange={(e) => changeRequestPoint(index, 2, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[3]} onChange={(e) => changeRequestPoint(index, 3, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[4]} onChange={(e) => changeRequestPoint(index, 4, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[5]} onChange={(e) => changeRequestPoint(index, 5, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[6]} onChange={(e) => changeRequestPoint(index, 6, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[7]} onChange={(e) => changeRequestPoint(index, 7, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[8]} onChange={(e) => changeRequestPoint(index, 8, e)} step={20}></input> </> :
							(val.type === "circle" || val.type === "sphere") ? <> <input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[0]} onChange={(e) => changeRequestPoint(index, 0, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[1]} onChange={(e) => changeRequestPoint(index, 1, e)} step={20}></input>
								<input className={inputItem + " col-span-1"} type={"number"} value={val.inputPoints[2]} onChange={(e) => changeRequestPoint(index, 2, e)} step={20}></input>
								<input className={inputItem + " col-span-3"} type={"number"} value={val.inputPoints[3]} onChange={(e) => changeRequestPoint(index, 3, e)} step={20}></input> </> : <></>}
					</div>
					<div className={inputItem + " grid gap-2 grid-cols-3 col-span-2"}>
						<h3 className={"col-span-3 text-center"}>Rotations List</h3>
						<input className={inputItem + " col-span-1"} type={"number"} value={val.roll} onChange={(e) => changeRequestRoll(index, e)} step={20}></input>
						<input className={inputItem + " col-span-1"} type={"number"} value={val.pitch} onChange={(e) => changeRequestPitch(index, e)} step={20}></input>
						<input className={inputItem + " col-span-1"} type={"number"} value={val.yaw} onChange={(e) => changeRequestYaw(index, e)} step={20}></input>
					</div>
					<button className={inputItem + " col-span-2 text-center cursor-pointer"} onClick={() => removeObject(index)}>Delete Object</button>
				</div>)
		})}
		<button className={inputItem + " col-span-3 text-center cursor-pointer"} onClick={newObject}>Add New Object</button>
		<button className={inputItem + " mt-2 col-span-3 text-center cursor-pointer"}>Render</button>
	</div>);
}

export default DemoControls;