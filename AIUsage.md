# AI Usage in Open Render
## Source Code
### open_render.hpp
- #### Added ```clearCanvas()``` to fix error with pixels persisting through re-renders relvant to the demo page.
## Website
### server.cpp
- #### ~~Fixed  bugs relating to ```<nlohmann/json> from_json()``` parsing overloads, I forgot to add the inline keyword and put some overloads in a specific namespace~~