import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import './style.css';
import Navbar from './Navbar.tsx';
import DemoCanvas from './demos/DemoCanvas.tsx';
import DemoControls from './demos/DemoControls.tsx';

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <Navbar location={"Demos"} />
    <div className={"flex items-center justify-items h-[calc(100vh-4rem)] mt-16 bg-gray-500"}>
      <DemoCanvas />
      <DemoControls />
    </div>
  </StrictMode>,
);