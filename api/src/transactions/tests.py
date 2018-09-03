from django.test import TestCase
from rest_framework.test import APILiveServerTestCase, APITestCase
import requests
import json
from django.contrib.auth.models import User
from transactions.models import Transaction


class UserLoginTokenTestCase(APILiveServerTestCase):

    def get_url_server(self):
        return self.live_server_url+"/"
    
    def setUp(self):
        self.username = "john"
        self.email = "john@snow.com"
        self.password = "you_know_nothing"
        self.user = User.objects.create_user(self.username, self.email, self.password)
    
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
        self.token = response.json()
        #print(self.token['access'])  
    
class TransactionsTestCase(UserLoginTokenTestCase):

    def get_url_server(self):
        return self.live_server_url+"/"
    
        """
        So create user
        """
    def setUp(self):
        self.username = "john"
        self.email = "john@snow.com"
        self.password = "you_know_nothing"
        self.user = User.objects.create_user(self.username, self.email, self.password)
        self.test_authentication_with_valid_data()
    
        """
        Test to verify the create a token
        """    
    def get_token(self):
        url = self.get_url_server()+"api/auth/token/"
        data = {"username":self.username,"password": self.password}
        response = requests.post(url,json=data)
        self.assertEqual(200, response.status_code)
        self.assertTrue("access" in json.loads(response.content))
        self.auth = response.json()
        self.token = self.auth['access']
        #print(self.token)

    def test_register_transaction(self):
        """
        Get 'access' token 
        """
        self.get_token()
        self.token = self.auth['access']

        """
        Test to verify the create a transaction
        """
        url = self.get_url_server()+"api/trading/transactions/"
        header = {'Authorization':'Bearer '+str(self.token)}
        #print(header)
        response = requests.post(url,headers=header)
        self.assertEqual(201, response.status_code)

    def test_query_all_transaction(self):
        """
        Get 'access' token 
        """
        self.get_token()
        self.token = self.auth['access']

        self.test_register_transaction()
        self.test_register_transaction()
        
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

    def test_register_detail_transaction(self):
        """
        Get 'access' token 
        """
        self.get_token()
        self.token = self.auth['access']

        self.test_register_transaction()

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


