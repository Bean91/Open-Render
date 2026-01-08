import React, { useEffect, useRef } from "react";
import "../style.css";

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

const DemoCanvas: React.FC<{requestData: request[], roll: number, pitch: number, yaw: number, x: number, y: number, z: number}> = ({ requestData, roll, pitch, yaw, x, y, z}) => {
	const canvasRef = useRef<HTMLCanvasElement>(null);

	useEffect(() => {
		(async () => {
			//@ts-ignore
			const j: any = {
				"transformations": {
					"roll": roll,
					"pitch": pitch,
					"yaw": yaw,
					"x": x,
					"y": y,
					"z": z,
				},
				"requests": {}
			}

			requestData.forEach((request: request) => {
				j["requests"][request.name] = {
					"type": request.type,
					"color": {
						"r": request.color.r,
						"g": request.color.g,
						"b": request.color.b,
						"a": request.color.a,
					},
					"points": {

					}
				}
				if (request.type === "line") {
					j["requests"][request.name]["points"] = {
						0: {
							"x": request.inputPoints[0],
							"y": request.inputPoints[1],
							"z": request.inputPoints[2],
						},
						1: {
							"x": request.inputPoints[3],
							"y": request.inputPoints[4],
							"z": request.inputPoints[5],
						}
					}
				} else if (request.type === "recPrFill" || request.type === "recPrOut") {
					j["requests"][request.name]["points"] = {
						0: {
							"x": request.inputPoints[0],
							"y": request.inputPoints[1],
							"z": request.inputPoints[2],
						},
						1: {
							"x": request.inputPoints[3],
							"y": request.inputPoints[4],
							"z": request.inputPoints[5],
						},
						2: {
							"x": request.roll,
							"y": request.pitch,
							"z": request.yaw,
						}
					}
				} else if (request.type === "triangle") {
					j["requests"][request.name]["points"] = {
						0: {
							"x": request.inputPoints[0],
							"y": request.inputPoints[1],
							"z": request.inputPoints[2],
						},
						1: {
							"x": request.inputPoints[3],
							"y": request.inputPoints[4],
							"z": request.inputPoints[5],
						},
						2: {
							"x": request.inputPoints[6],
							"y": request.inputPoints[7],
							"z": request.inputPoints[8],
						}
					}
				} else if (request.type === "sphere") {
					j["requests"][request.name]["points"] = {
						0: {
							"x": request.inputPoints[0],
							"y": request.inputPoints[1],
							"z": request.inputPoints[2],
						},
						1: {
							"x": request.inputPoints[3],
							"y": 0,
							"z": 0,
						}
					}
				} else if (request.type === "circle") {
					j["requests"][request.name]["points"] = {
						0: {
							"x": request.inputPoints[0],
							"y": request.inputPoints[1],
							"z": request.inputPoints[2],
						},
						1: {
							"x": request.inputPoints[3],
							"y": 0,
							"z": 0,
						},
						2: {
							"x": request.roll,
							"y": request.pitch,
							"z": request.yaw,
						}
					}
				}
			});
			console.log(j);

			try {
				console.log("try");
				const response = await fetch("http://127.0.0.1:8080/api/demo", {
					"method": "POST",
					"headers": {
						"Content-Type": "application/json",
						"Accept": "application/octet-stream, text/plain"
					},
					"body": JSON.stringify(j)
				});
				console.log(response);
				if (!response.ok) {
					throw new Error(`HTTP error! status: ${response.status}, response: ${response.body}`);
				}
				const arrayBuffer = await response.arrayBuffer();
				const pixels = new Uint8Array(arrayBuffer);
				console.log('Received binary data:', pixels);

				const canvas = canvasRef.current;
			    if (!canvas) return;
			    const ctx = canvas.getContext('2d');
			    if (!ctx) return;
			    const imageData = ctx.createImageData(width, height);
			    imageData.data.set(pixels);
				console.log(imageData);
				ctx.clearRect(0, 0, canvas.width, canvas.height);
			    ctx.putImageData(imageData, 0, 0);
			} catch(error) {
				console.error('Error fetching binary data:', error);
			}
		})();
	}, [requestData, roll, pitch, yaw, x, y, z]);

	const downloadCanvas = () => {
		const canvas = canvasRef.current;
		if (!canvas) return;

		const dataURL = canvas.toDataURL('image/png');

		const downloadLink = document.createElement('a');
		downloadLink.setAttribute('href', dataURL);
		downloadLink.setAttribute('download', 'image.png');

		document.body.appendChild(downloadLink);
		downloadLink.click();
		document.body.removeChild(downloadLink);
	};

	const width: number = 852;
	const height: number = 480;

	return <div className={"bg-gray-700 items-center rounded-md m-4 p-2"}><canvas width={width} height={height} ref={canvasRef} /><button className={"text-white rounded-sm bg-blue-600 p-2 py-1 m-1 cursor-pointer"} onClick={downloadCanvas}>Download Image</button></div>;
}

export default DemoCanvas;