from django.test import TestCase
from rest_framework.test import APILiveServerTestCase, APITestCase
import requests
import json
from api_trading.test import TradingBaseTestCase
from django.contrib.auth.models import User
from transactions.models import Transaction


class UserLoginTokenTestCase(TradingBaseTestCase):

    
    def setUp(self):
       self.setupUser();   
    def test_authentication_without_password(self):
        url = self.get_url_server()+"api/auth/token/"
        response = requests.post(url, {"username":"snowman"})
        self.assertEqual(400, response.status_code)
    
    def test_authentication_wrong_password(self):
        url = self.get_url_server()+"api/auth/token/"
        data = {"username":self.username,"password":"I_know"}
        response = requests.post(url, json=data)
        self.assertEqual(400, response.status_code)
    
    def test_authentication_with_valid_data(self):
        url = self.get_url_server()+"api/auth/token/"
        data = {"username":self.username,"password": self.password}
        response = requests.post(url,json=data)
        self.assertEqual(200, response.status_code)
        self.assertTrue("access" in json.loads(response.content))
    
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
        #print(header)
        response = requests.post(url,headers=header)
        self.assertEqual(201, response.status_code)

class TransactionQueryTestCase(TradingBaseTestCase):

    def setUp(self):
        self.setupUser();
        self.setupToken();
        for i in range(2):
            url = self.get_url_server()+"api/trading/transactions/"
            header = {'Authorization':'Bearer '+str(self.token)}
            response = requests.post(url,headers=header)
        
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
        response = requests.post(url,headers=header)
        self.transaction_url=response.json["url"]
    
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
            "transaction": self.get_url_server()+"api/trading/transactions/1/",
            "parity": "btc_usd",
            "amount": "55000.0000000000",
            "commission": "0.0200000000",
            "okex_order": 1
            }
        print(data)
        response = requests.post(url,headers=header,json=data)
        self.assertEqual(201, response.status_code)

