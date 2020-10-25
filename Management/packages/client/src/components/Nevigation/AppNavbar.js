import React from "react";
import AppBar from "@material-ui/core/AppBar";
import Toolbar from "@material-ui/core/Toolbar";
import IconButton from "@material-ui/core/IconButton";
import MenuIcon from "@material-ui/icons/Menu";
import MenuItem from "@material-ui/core/MenuItem";
import Menu from "@material-ui/core/Menu";
import Button from "@material-ui/core/Button";
import Box from "@material-ui/core/Box";


class AppNavbar extends React.Component {
 
  constructor() {
    super();
    this.state = {
      isAuthenticated: true,
      anchorEl: null,
    };
  }

  handleMenu = (event) => {
    this.setState({ anchorEl: event.currentTarget });
  };

  handleClose = () => {
    this.setState({ anchorEl: null });
  };

  render() {
    return (
      <React.Fragment>
        <AppBar className="flex-grow flex bg-gray-800  " position="static"  >
          <Toolbar>
            <IconButton
              edge="start"
              className="mr-2  "
              color="inherit"
              aria-label="menu"
              onClick={() => {}}
            >
              <MenuIcon />
            </IconButton>
            <Box>
                <Button
                  className="flex-auto h-8 m-2 bg-gray-600 border-double border-4 border-gray-400 font-mono text-white"
                  size = "small" 
                  variant="contained" 
                >
                  Stresser
                </Button>
              <Button 
                className="flex-auto h-8 m-2 bg-gray-600 border-double border-4 border-gray-400 font-mono text-white"
                size = "small" 
                variant="contained" 
              >
                End-point
              </Button>
              <Button 
                className="flex-auto h-8 m-2 bg-gray-600 border-double border-4 border-gray-400 font-mono text-white"
                size = "small" 
                variant="contained" 
              >
                Policy
              </Button>
              <Button 
                className="flex-auto h-8 m-2 bg-gray-600 border-double border-4 border-gray-400 font-mono text-white"
                size = "small" 
               variant="contained" 
              >
                Rules
              </Button>
              <Button  
                className="flex-auto h-8 m-2 bg-gray-600 border-double border-4 border-gray-400 font-mono text-white"
                size = "small" 
                variant ="contained" 
                >
                Events
              </Button>
              </Box>
               <Button 
                className="absolute right-0 h-8 ml-16 bg-gray-600 border-double border-4 border-gray-400 font-mono text-white"
                size = "small" 
                variant="contained"
              >
                Log-out
              </Button>


            {/* <Menu
              id="menu-appbar"
              anchorEl={this.state.anchorEl}
              anchorOrigin={{
                vertical: "top",
                horizontal: "right",
              }}
              keepMounted
              transformOrigin={{
                vertical: "top",
                horizontal: "right",
              }}
              open={this.state.anchorEl ? true : false}
              onClose={() => this.handleClose()}
            >
              <MenuItem onClick={this.handleClose}>Profile</MenuItem>
              <MenuItem onClick={this.handleClose}>My account</MenuItem>
            </Menu> */}
          </Toolbar>
        </AppBar>
      </React.Fragment>
    );
  }
}

export default AppNavbar;
