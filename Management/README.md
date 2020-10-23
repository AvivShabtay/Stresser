# Crowd Detector

<p align="left">
    <img src="https://img.shields.io/badge/%F0%9F%91%AB-4_Team_Members-1d2935.svg">
    <img src="https://img.shields.io/badge/JAVA-v8+-blue.svg">
    <img src="https://img.shields.io/badge/license-MIT-blue.svg">
    <img src="https://img.shields.io/badge/contributions-welcome-orange.svg">
</p>

## Contributors
* [Aviv Shabtay](https://github.com/AvivShabtay)
* [Alon Bukai](https://github.com/Alonski)
* [Elior Shriki](https://github.com/EliorShriki)
* [Or Pinhas](https://github.com/orpinhas20)

## Purpose
Fight the spread of COVID-19 virus by allow users to identify whether they are in a crowd.

## Demo
|                   Sign-in                    |                  Risks                   |
| :------------------------------------------: | :--------------------------------------: |
| <img src="images/signin_demo.gif" width=300> | <img src="images/map_red.gif" width=300> |

## Architecture

This application is split into a server (Java Spring) and a client (React).

## Deployments

Deploying is handled automatically on every push to Master.
The server is deployed at: https://crowd-detector.herokuapp.com/
The client is deployed at: https://crowd-detector.netlify.app/

## Testing

Unit tests on the server are run by Maven.

## Error Tracking

We use Sentry.io for error tracking:
https://sentry.io/organizations/crowd-detector/issues/?project=5251243

## Generate Mock Users + Locations

### Generate Users

To generate users you can use the node script `mock:users`
It can receive an arg for AMOUNT: `yarn mock:users 100`

### Generate Locations

To generate locations you can use the node script `mock:locations`
It can receive args for LAT, LNG, RADIUS, SPEED, AMOUNT: `yarn mock:locations 32.4675101 34.9547147 300 200 100`

## TODO
- [ ]  Convert BitBucket Pipelines into GitHub Actions
- [ ]  Handle identity & security issues
- [ ]  Register Heroku cloud as open source project (free hosting)
