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
			const j = {
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
				} else if (request.type === "recPrFill" || request.type === "recPrOut" || request.type === "triangle") {
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
							"y": request.inputPoints[4],
							"z": request.inputPoints[5],
						},
						2: {
							"x": request.inputPoints[6],
							"y": 0,
							"z": 0,
						}
					}
				}
			});
			console.log(j);

			try {
				const response = await fetch("0.0.0.0:8080/demo", {
					"method": "POST",
					"headers": {
						"Content-Type": "application/json",
						"Accept": "application/octet-stream, text/plain"
					},
					"body": JSON.stringify(j)
				});
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
			    imageData.data.set(new Uint8ClampedArray(pixels));
			    ctx.putImageData(imageData, 0, 0);
			} catch(error) {
				console.error('Error fetching binary data:', error);
			}
		})();
	}, [requestData, roll, pitch, yaw, x, y, z]);

	const width: number = 852;
	const height: number = 480;

	return <div className={"bg-gray-700 flex items-center rounded-md m-4 p-2"}><canvas width={width} height={height} ref={canvasRef} /></div>;
}

export default DemoCanvas;