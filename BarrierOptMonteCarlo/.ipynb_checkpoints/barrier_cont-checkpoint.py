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

def discount_down_out_cont(price, s0, strike, barrier):
    n_samples = len(price[-1, :])
    call_price_discount = (1-pc(s0, price[-1, :], barrier))*np.maximum(np.zeros(n_samples),price[-1, :]-strike)
    put_price_discount = (1-pc(s0, price[-1, :], barrier))*np.maximum(np.zeros(n_samples),-price[-1, :]+strike) 
    return call_price_discount.mean(), put_price_discount.mean()
   

def to_float(vec):
    for i in range(0, len(vec)): 
        vec[i] = float(vec[i])
    return vec

def pc(s0, sT, barrier):
    out = (sT >= barrier)*np.exp(-2*np.log(s0/barrier)*np.log(sT/barrier)/(vol**2*T))
    out = np.where(out==0, 1, out)
    return out


def vanilla(s0, strike, r, vol, T):
#     ncdf = normal.cdf
    d1 = (np.log(s0/strike)+T*(r+(1/2)*vol**2))/(vol*np.sqrt(T))
    d2 = (np.log(s0/strike)+T*(r-(1/2)*vol**2))/(vol*np.sqrt(T))
    call = s0*ncdf(d1)-strike*np.exp(-r*T)*ncdf(d2)
    put = strike*np.exp(-r*T)*ncdf(-d2)-s0*ncdf(-d1)
    return call, put

def theoretical_down_out(s0, barrier, strike, r, vol, T):
    call, put = vanilla(s0, strike, r, vol, T)
    cdi, pdi = theoretical_down_in(s0, barrier, strike, r, vol, T)
    return call-cdi, put-pdi

def theoretical_down_in(s0, barrier, strike, r, vol, T):
#     ncdf = normal.cdf
    lam = (r+(vol)**2/2)/vol**2
    x1 = np.log(s0/barrier)/(vol*np.sqrt(T))+lam*vol*np.sqrt(T)
    y1 = np.log(barrier/s0)/(vol*np.sqrt(T))+lam*vol*np.sqrt(T)
    y = np.log(barrier**2/(s0*strike))/(vol*np.sqrt(T))+lam*vol*np.sqrt(T)
    cdi = s0 *ncdf(y)*(barrier/s0)**(2*lam)-strike*np.exp(-r*T)*ncdf(y-vol*np.sqrt(T))*(barrier/s0)**(2*lam-2)
    pdi = -s0 *ncdf(-x1)+strike*np.exp(-r*T)*ncdf(-x1+vol*np.sqrt(T))+s0*(barrier/s0)**(2*lam)*(ncdf(y)-ncdf(y1))-strike*np.exp(-r*T)*(barrier/s0)**(2*lam-2)*(ncdf(y-vol*np.sqrt(T))-ncdf(y1-vol*np.sqrt(T)))
    return cdi, pdi


if __name__ == "__main__":
    start = time.time()
    T, r, vol, s0, strike, n_steps, n_samples, barrier = to_float(sys.argv[1:]) 
    n_steps = int(n_steps)
    n_samples = int(n_samples)
    call_price, put_price, price = monte_carlo_down_out(T, r, vol, s0, strike, n_steps, n_samples, barrier)
    call_price_discount, put_price_discount = discount_down_out_cont(price, s0, strike, barrier)
    call_price_theo, put_price_theo = theoretical_down_out(s0, barrier, strike, r, vol, T)
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
    print("European down and out continuous barrier option pricing via Monte Carlo simulation")
    print("Monte Carlo call price: ", call_price)
    print("Monte Carlo put price: ", put_price)
    print("(1-pc) discount call price: ", call_price_discount.mean())
    print("(1-pc) discount put price: ", put_price_discount.mean())
    print("Theoretical call price: ", call_price_theo)
    print("Theoretical put price: ", put_price_theo)
    print("--------------------------------------------------------------")
    end = time.time()
    print("Simulation time:", round(end-start, 4), "seconds")