import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import './style.css';
import Navbar from './Navbar.tsx';
import DemoControls from './demos/DemoControls.tsx';

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <Navbar location={"Demos"} />
    <DemoControls />
  </StrictMode>,
);