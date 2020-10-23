import React from "react";
import "./App.css";
//import { Route } from 'react-router-dom';
import AppNavbar from "./components/Nevigation/AppNavbar";
import "./tailwind.generated.css";

const App = () => {
  return (
    <div className="text-center">
      <AppNavbar />
    </div>
  );
};

export default App;
