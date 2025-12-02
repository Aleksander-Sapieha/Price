import requests

URL = "https://api.nbp.pl/api/exchangerates/rates/c/usd/today/"

def changeCurrency(price):
    response = requests.get(URL).json()
    askPrice = response["rates"][0]["ask"]
    return price / askPrice

if __name__ == "__main__":
    print(changeCurrency(100))