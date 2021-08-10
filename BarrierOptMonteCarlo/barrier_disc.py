import sys
import time
import numpy as np
from scipy.stats import norm
ncdf = norm.cdf


def wiener_proc(N_samples, delta):
    dRW = np.random.normal(0, np.sqrt(delta),N_samples)
    dRW[0] = 0
    return np.cumsum(dRW, axis=0)


def geometric_wiener_vec(w, s0, r, vol, n, t):
    w_shape = np.shape(w) 
    s = np.zeros(w_shape)
    for i in range(0, n):
        s[i, :] = s0*np.exp((r-vol**2/2)*t[i]+vol*w[i, :])
    return s

def monte_carlo_down_out(T, r, vol, s0, strike, n_steps, n_samples, barrier):
    t = np.linspace(0., T, n_steps)
    price = geometric_wiener_vec(wiener_proc([n_steps, n_samples], abs(t[1]-t[0])), s0, r, vol, n_steps, t)
    put_price = np.sum((price>=barrier).prod(axis=0)*np.maximum(np.zeros(n_samples),-price[-1, :]+strike))/n_samples
    call_price = np.sum((price>=barrier).prod(axis=0)*np.maximum(np.zeros(n_samples),price[-1, :]-strike))/n_samples
    return call_price, put_price, price

   
def discount_down_out_disc(price, t, s0, strike, barrier):
    n_samples = len(price[-1, :])
    call_price_discount =(pd( s0, price[-1, :], barrier, t))*np.maximum(np.zeros(n_samples),price[-1, :]-strike)
    put_price_discount = (pd( s0, price[-1, :], barrier, t))*np.maximum(np.zeros(n_samples),-price[-1, :]+strike)
    return call_price_discount.mean(), put_price_discount.mean()
   

def to_float(vec):
    for i in range(0, len(vec)): 
        vec[i] = float(vec[i])
    return vec


def pd(s0, sT, barrier, t):
    mu = s0+(t*np.ones([len(t),len(sT)]).dot(sT-s0)-t[0])/(t[-1]-t[0])
    var = (t-t[0])*(t[-1]-t)/(t[-1]-t[0])
    mu = mu[1:-2]
    var = var[1:-2]
    return (1-norm.cdf((barrier-mu)/np.sqrt(var))).prod(axis=0)


if __name__ == "__main__":
    start = time.time()
    T, r, vol, s0, strike, n_steps, n_samples, barrier = to_float(sys.argv[1:]) 
    n_steps = int(n_steps)
    n_samples = int(n_samples)
    t = np.linspace(0, T, n_steps)
    call_price, put_price, price = monte_carlo_down_out(T, r, vol, s0, strike, n_steps, n_samples, barrier)
    call_price_discount, put_price_discount = discount_down_out_disc(price, t, s0, strike, barrier)
    print("--------------------------------------------------------------")
    print("Expiration time in years: ", T)
    print("Risk free rate: ",r)
    print("Volatility: ", vol)
    print("Initial price of underlying: ", s0)
    print("Barrier price: ", barrier)
    print("Strike price: ",strike)
    print("Number of trials: ", n_samples)
    print("Number of time steps: ", n_steps)
    print("--------------------------------------------------------------")
    print("European down and out discrete barrier option pricing via Monte Carlo simulation")
    print("Monte Carlo call price: ", call_price)
    print("Monte Carlo put price: ", put_price)
    print("Brownian bridge call price: ", call_price_discount)
    print("Brownian bridge put price: ", put_price_discount)
    print("--------------------------------------------------------------")
    end = time.time()
    print("Simulation time:", round(end-start, 4), "seconds")