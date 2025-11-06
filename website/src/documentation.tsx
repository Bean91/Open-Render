import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import './style.css';
import Navbar from './navbar.tsx';

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <Navbar location={"Documentation"} />
  </StrictMode>,
);