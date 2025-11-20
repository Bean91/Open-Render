import React, { useEffect, useRef } from "react";
import "../style.css";

const DemoCanvas: React.FC = () => {
	const width: number = 852;
	const height: number = 480;
	const canvasRef = useRef(null);

	useEffect(() => {
		const canvas = canvasRef.current;
		const ctx = canvas.getContext('2d');

		const imageData = ctx.createImageData(width, height);
		const pixelData = imageData.data;

		ctx.putImageData(imageData, 0, 0);
	}, []);

	return <div className={"bg-gray-700 flex items-center rounded-md m-4 p-2"}><canvas ref={canvasRef} width={width} height={height} /></div>;
}

export default DemoCanvas