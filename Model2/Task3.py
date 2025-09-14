import numpy as np
import matplotlib.pyplot as plt

def plot_convergence(costs, learning_rate):
    """Plot the cost function convergence"""
    plt.figure(figsize=(12, 4))
    
    plt.subplot(1, 2, 1)
    plt.plot(costs)
    plt.title(f'Cost Function Convergence\n(Learning Rate = {learning_rate})')
    plt.xlabel('Iteration')
    plt.ylabel('MSE Cost')
    plt.grid(True)
    
    #Plot 2: Log scale to see convergence better
    plt.subplot(1, 2, 2)
    plt.semilogy(costs)
    plt.title('Cost Convergence (Log Scale)')
    plt.xlabel('Iteration')
    plt.ylabel('MSE Cost (log scale)')
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def plot_data_and_fit(X, Y, W, b):
    """Plot the original data and the fitted line"""
    plt.figure(figsize=(8, 6))
    
    # Scatter plot of data points
    plt.scatter(X.flatten(), Y.flatten(), alpha=0.6, color='blue', label='Data points')
    
    # Plot the fitted line
    X_sorted = np.sort(X.flatten())
    Y_line = W[0,0] * X_sorted + b[0,0]
    plt.plot(X_sorted, Y_line, 'r-', linewidth=2, label=f'Fitted line: y = {W[0,0]:.2f}x + {b[0,0]:.2f}')
    
    # Plot the true relationship
    Y_true = 3 * X_sorted + 2
    plt.plot(X_sorted, Y_true, 'g--', linewidth=2, label='True relationship: y = 3x + 2')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Linear Regression: Data and Fitted Line')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.show()




def linear_forward(A, W, b):
    Z = np.dot(W, A) + b
    cache = (A, W, b)
    return Z, cache

def compute_mse_cost(Y_hat, Y):
    m = Y.shape[1]
    cost = (1 / (2 * m)) * np.sum((Y_hat - Y) ** 2)
    return cost

def linear_backward(dZ, cache):
    A_prev, W, b = cache
    m = A_prev.shape[1]
    dW = (1 / m) * np.dot(dZ, A_prev.T)
    db = (1 / m) * np.sum(dZ, axis=1, keepdims=True)
    dA_prev = np.dot(W.T, dZ)
    return dA_prev, dW, db

def update_parameters(W, b, dW, db, learning_rate):
    W -= learning_rate * dW
    b -= learning_rate * db
    return W, b

def simple_ass_linear_regression(X, Y, learning_rate=0.1, num_iterations=10000):
    W = np.random.randn(1, X.shape[0]) * 0.01
    b = np.zeros((1, 1))
    costs = []
    for i in range(num_iterations):
        #forward part
        Z, cache = linear_forward(X, W, b)
        Y_hat = Z
        cost = compute_mse_cost(Y_hat, Y)
        costs.append(cost)
        #backward part
        m = Y.shape[1]
        dZ = (1 / m) * (Y_hat - Y)
        dA_prev, dW, db = linear_backward(dZ, cache)
        #update parameters
        W, b = update_parameters(W, b, dW, db, learning_rate)
    return W, b, costs

if __name__ == "__main__":
    #init of the data
    np.random.seed(42)
    m = 100
    X = np.random.randn(1, m)
    Y = 3 * X + 2 + np.random.randn(1, m) * 0.1
    #trianing
    W, b, costs = simple_ass_linear_regression(X, Y)
    #test after finishing training (inference)
    Y_pred, _ = linear_forward(X, W, b)
    final_cost = compute_mse_cost(Y_pred, Y)
    print(f"Final cost: {final_cost}")
    plot_convergence(costs, 0.1)
    plot_data_and_fit(X, Y, W, b)
