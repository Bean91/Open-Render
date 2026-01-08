import './style.css';
import Navbar from './Navbar.tsx';
import DemoControls from './demos/DemoControls.tsx';
import { useTitle } from "./hooks/useTitle.ts";

export default function Demos() {
  useTitle("Demos | Open Render");

  return (
      <div>
        <Navbar location={"Demos"} />
        <DemoControls />
      </div>
    )
}