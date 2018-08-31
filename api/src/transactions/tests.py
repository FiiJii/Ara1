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
    
