import React, {useState} from "react";
import "./style.css";

function Navbar() {
	const [place, setPlace] = useState("Home")
	const items: string[] = ["Home", "Documentation", "Tests"]
	const itemClass: string = "p-4 bg-gray-400 h-12 m-2";

	const changePlace = (item: string) => {
		setPlace(item);
	}

	return (
		<div className="flex bg-gray-600 h-16 justify-between">
			{items.map((item: string, key: number) => (<div className={place === item ? "bg-gray-200" + itemClass : itemClass} key={key}><p onClick={changePlace(item)} key={key}>{item}</p></div>))}
		</div>
	);
}

export default Navbar