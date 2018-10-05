from django.test import TestCase
from rest_framework.test import APILiveServerTestCase, APITestCase
import requests
import json
from api_trading.test import TradingBaseTestCase
from django.contrib.auth.models import User
from transactions.models import Transaction


    
class TransactionsTestCase(TradingBaseTestCase):

    def setUp(self):
        self.setupUser();
        self.setupToken();
        """
        Test to verify the create a token
        """    
        #print(self.token)

    def test_register_transaction(self):
        """
        Test to verify the create a transaction
        """
        url = self.get_url_server()+"api/trading/transactions/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "investment":2000,
            "earnings": 2250
            }
        #print(header)
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(201, response.status_code)
        self.assertTrue(('id', 'url', 'creation_date','investment','earnings' in json.loads(response.content)))


class TransactionQueryTestCase(TradingBaseTestCase):

    def setUp(self):
        self.setupUser();
        self.setupToken();
        url = self.get_url_server()+"api/trading/transactions/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data1 = {
            "investment":2000,
            "earnings": 2250
            }
        data2 = {
            "investment":50000,
            "earnings": 15500
            }

        response = requests.post(url,headers=header,json=data1)
        response = requests.post(url,headers=header,json=data2)
        
    def test_query_all_transaction(self):
        """
        Test to verify transactions queries already created
        """
        url = self.get_url_server()+"api/trading/transactions/"
        header = {'Authorization':'Bearer '+str(self.token)}
        #print(header)
        response = requests.get(url,headers=header)
        self.assertEqual(200, response.status_code)
        data = response.json()
        #print(data)
        self.assertEqual(data["count"],2)



class TransactionDetailsTestCase(TradingBaseTestCase):

    def setUp(self):
        self.setupUser();
        self.setupToken();
        url = self.get_url_server()+"api/trading/transactions/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "investment":2000,
            "earnings": 2250
            }
        response = requests.post(url,headers=header,json=data)
        self.transaction_url=response.json()["url"]
    
    def test_register_detail_transaction(self):
        """
        Get 'access' token 
        """
        self.setupToken()
        self.token = self.auth['access']


        url = self.get_url_server()+"api/trading/transaction_details/"
        header = {'Authorization':'Bearer '+str(self.token)}
        print(header)
        data = {
            "transaction": self.transaction_url,
            "parity": "btc_usd",
            "amount": "55000.0000000000",
            "commission": "0.0200000000",
            "okex_order": 1,
            "transaction_order": 1
            }
        print(data)
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(201, response.status_code)

class TransactionDetailsQueryTestCase(TradingBaseTestCase):

    def setUp(self):
        self.setupUser();
        self.setupToken();
        url = self.get_url_server()+"api/trading/transactions/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data = {
            "investment":2000,
            "earnings": 2250
            }
        response = requests.post(url,headers=header,json=data)
        self.transaction_url=response.json()["url"]


        url = self.get_url_server()+"api/trading/transaction_details/"
        header = {'Authorization':'Bearer '+str(self.token)}
        data1 = {
            "transaction": self.transaction_url,
            "parity": "btc_usd",
            "amount": "55000.0000000000",
            "commission": "0.0200000000",
            "okex_order": 1060,
            "transaction_order": 1
            }
        data2 = {
            "transaction": self.transaction_url,
            "parity": "usd_ltc",
            "amount": "60000.0000000000",
            "commission": "0.3200000000",
            "okex_order": 1061,
            "transaction_order": 2
            }
        
        response = requests.post(url,headers=header,json=data1)
        response = requests.post(url,headers=header,json=data2)
    
    def test_query_all_transaction_detail(self):
        """
        Test to verify transactions detail queries already created
        """
        url = self.get_url_server()+"api/trading/transaction_details/"
        header = {'Authorization':'Bearer '+str(self.token)}
        response = requests.get(url,headers=header)
        self.assertEqual(200, response.status_code)
        data = response.json()
        self.assertEqual(data["count"],2) 