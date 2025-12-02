import requests

URL = "https://api.nbp.pl/api/cenyzlota"

print(requests.get(URL).json())