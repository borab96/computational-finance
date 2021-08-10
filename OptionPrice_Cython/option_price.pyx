from libc.math cimport sqrt, exp, pow




# cache = {}
# cdef float american_call(int k, int i, float price, float strike_price, float vol, float expiration_time, float risk_free_rate, int n_div = 30) except? -2: 
#     cdef float u = exp(vol*sqrt(2*expiration_time/(float(n_div))))
#     cdef float d =1/u
#     cdef float R = exp(risk_free_rate*expiration_time/(float(n_div)))
#     cdef float pu = pow((sqrt(R) - sqrt(d))/(sqrt(u)-sqrt(d)), 2)
#     cdef float pd = pow((sqrt(u)-sqrt(R))/(sqrt(u)-1/sqrt(u)),2)
#     ins = (k, i, price)
#     if ins in cache:
#         return cache[ins]
#     if k == n_div:
#         return max(0.0, price-strike_price)
#     else:
#         val = max(price-strike_price,(pu*american_call(k+1, i+1, price*u, strike_price,vol, expiration_time, risk_free_rate, n_div=n_div)+(1-pu-pd)*american_call(k+1, i, price,  strike_price,vol, expiration_time, risk_free_rate, n_div=n_div)+pd*american_call(k+1, i-1, price*d,  strike_price,vol, expiration_time, risk_free_rate, n_div=n_div))/R)
#         cache[ins] = val
#         return val
    
cache = {} 
cdef class Option:
    
    cdef float volatility
    cdef float expiration_time
    cdef int no_of_divisions 
    cdef float risk_free_rate
    cdef float strike_price 
    cdef float u
    cdef float d 
    cdef float R
    cdef float pu
    cdef float pd 
   
    
    def __cinit__(self, float strike, float vol, float exp_time, float r=0.01):
       
        self.volatility = vol
        self.expiration_time = exp_time
        self.no_of_divisions = 25
        self.risk_free_rate = r
        self.strike_price = strike
        self.u = exp(self.volatility*sqrt(2*self.expiration_time/(float(self.no_of_divisions))))
        self.d =1/self.u
        self.R = exp(self.risk_free_rate*self.expiration_time/(float(self.no_of_divisions)))
        self.pu = pow((sqrt(self.R) - sqrt(self.d))/(sqrt(self.u)-sqrt(self.d)), 2)
        self.pd = pow((sqrt(self.u)-sqrt(self.R))/(sqrt(self.u)-1/sqrt(self.u)),2)

    def call(self, int k, int i, float price): 
        global cache
        ins = (k, i, price)
        if ins in cache:
            return cache[ins]
        if k == self.no_of_divisions:
#             cache.clear()
            return max(0.0, price-self.strike_price)
        else:
            val = max(price-self.strike_price,(self.pu*self.call(k+1, i+1, price*self.u)+(1-self.pu-self.pd)*self.call(k+1, i, price)+self.pd*self.call(k+1, i-1, price*self.d))/self.R)
            cache[ins] = val
            return val
        
    def put(self, int k, int i, float price): 
        global cache
        ins = (k, i, price)
        if ins in cache:
            return cache[ins]
        if k == self.no_of_divisions:
            return max(0.0, -(price-self.strike_price))
        else:
            val = max(-price+self.strike_price,(self.pu*self.put(k+1, i+1, price*self.u)+(1-self.pu-self.pd)*self.put(k+1, i, price)+self.pd*self.put(k+1, i-1, price*self.d))/self.R)
            cache[ins] = val
            return val