Backgournd:
1. Layered architecture: Router -> Controller <-> Service Layer (BL) <-> Data Access Layer (models)
2. Solid - Single Responsibility.
3. EventEmitter pattern to send events between services.

Router:
1. Extra layer, can be merged with the Controllers.
2. Proxy the API calls (URI) to the relevant Controller.

Controllers:
- Handle requests to the application
- Validate the incoming data from the client.
- Call the Services to process data
- Return the results to the user.

Services:
- The business logic lived here.
- Accept call to manipulate data.
- Convert between outside data (Boundary) to inside data (Entity).
- Calls the DAL for operations.
- Return the results to the Controllers.

Rules:
1. Don't put your business logic inside the controllers - causes spaghetti code.
2. Preform data validation using middleware or JOI (in the controller or routes - using middleware).
3. The end of the controller handling request is when the response send to the client.
4. Distinguish when the request should continue processing in 'background'.
5. Don't pass req/res objects to service layer.
6. Use Event Emitter to reduce dependence between Services.

Links:
1. https://dev.to/santypk4/bulletproof-node-js-project-architecture-4epf
2. https://github.com/AlaaMezian/NodeJs-backend-structure

Make it work:
```shell
nodemon app.js
```
