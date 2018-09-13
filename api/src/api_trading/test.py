from django.test import TestCase
from rest_framework.test import APILiveServerTestCase
import requests
import json
from django.contrib.auth.models import User


class TradingBaseTestCase(APILiveServerTestCase):

    def get_url_server(self):
        return self.live_server_url+"/"
    
    def setupUser(self):
        self.username = "john"
        self.email = "john@snow.com"
        self.password = "you_know_nothing"
        self.user = User.objects.create_user(self.username, self.email, self.password)

    def setupToken(self):
        url = self.get_url_server()+"api/auth/token/"
        data = {"username":self.username,"password": self.password}
        response = requests.post(url,json=data)
        self.assertEqual(200, response.status_code)
        self.assertTrue("access" in json.loads(response.content))
        self.auth = response.json()
        self.token = self.auth['access']
 
    