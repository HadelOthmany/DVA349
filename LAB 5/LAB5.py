import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

input_size = 784
hidden_size = 50   # hidden layer
output_size = 10
learning_rate = 0.1
epochs = 15
batch_size = 50
train_losses = []
val_accuracies = []


# Load and prepare data

df= pd.read_csv("assignment5.csv")
table_data= df.values
X = table_data[:, 1:].astype(np.float32) / 255.0
y = table_data[:, 0].astype(int)

def one_hot_encode(labels, total_classes):
    encoded = np.zeros((labels.shape[0], total_classes))
    encoded[np.arange(labels.shape[0]), labels] = 1
    return encoded

Y = one_hot_encode(y, 10)

# Shuffle manually
np.random.seed(41) # fixed seed for reproducibility 
positions = np.random.permutation(len(X))
# Split manually: 70% train, 10% validation, 20% test
total_samples = len(X)

train_stop = int(0.7 * total_samples)         # End of training data (70%)
validation_stop = int(0.8 * total_samples)    # End of validation data (80%)

# Inputs
X_train = X[:train_stop]                     
X_validation = X[train_stop:validation_stop]  
X_test = X[validation_stop:]                 

# One-hot labels
Y_train = Y[:train_stop]
Y_validation = Y[train_stop:validation_stop]
Y_test = Y[validation_stop:]

# Original labels
y_train = y[:train_stop]
y_validation = y[train_stop:validation_stop]
y_test = y[validation_stop:]


# Activation functions

def relu_activation(input_values):                                         
    relu_result = np.maximum(0, input_values)                             
    return relu_result                                                   


def relu_gradient(input_values):                                           
    relu_derivative_result = (input_values > 0).astype(float)             
    return relu_derivative_result                                        


def softmax_activation(input_values):                                     
    shifted_values = input_values - np.max(input_values, axis=1, keepdims=True)      

    exponent_values = np.exp(shifted_values)                       
    sum_exponent_values = np.sum(exponent_values, axis=1, keepdims=True)      
    softmax_result = exponent_values / sum_exponent_values          
         
    
    return softmax_result                                                


# Loss and accuracy

def calculate_cross_entropy(real_output, predicted_output):                         
    # Small value to avoid log(0)
    epsilon = 1e-12                                                       
    
    safe_predictions = np.clip(predicted_output, epsilon, 1 - epsilon)    
    log_values = np.log(safe_predictions)                                 
    
    _values = real_output * log_values                          
    
    sum_of_values= np.sum(_values, axis=1)                     
    
    # Take mean and apply negative sign to get final loss
    mean_loss = -np.mean(sum_of_values)                                   
    
    return mean_loss                                                       # Return average cross-entropy loss


def calculate_accuracy(real_labels, predicted_probabilities):               
    predicted_classes = np.argmax(predicted_probabilities, axis=1)        
    
    correct_predictions = predicted_classes == real_labels                
    
    # Compute mean accuracy
    accuracy_result = np.mean(correct_predictions)                        
    
    return accuracy_result                                                 # Return accuracy value

np.random.seed(41) # fixed seed for reproducibility
weight1 = np.random.randn(input_size, hidden_size) * 0.01
bias1 = np.zeros((1, hidden_size))

weight2 = np.random.randn(hidden_size, output_size) * 0.01
bias2 = np.zeros((1, output_size))



#  Training loop
for epoch in range(epochs):
    # Shuffle training data every epoch
    indices = np.random.permutation(len(X_train))
    X_train = X_train[indices]
    Y_train = Y_train[indices]
    y_train = y_train[indices]

    for start in range(0, len(X_train), batch_size):
        end = start + batch_size
        X_batch = X_train[start:end]
        Y_batch = Y_train[start:end]

        # Forward pass
        h_in = np.dot(X_batch, weight1) + bias1
        h_out = relu_activation(h_in)                  # hidden layer activation

        out_layer_in = np.dot(h_out, weight2) + bias2
        prob = softmax_activation(out_layer_in)               # output probabilities

        # Backpropagation
        m = X_batch.shape[0]

        out_err = (prob - Y_batch) / m
        gradient_h = np.dot(h_out.T, out_err)
        gradient_bias_h = np.sum(out_err, axis=0, keepdims=True)

        h_err = np.dot(out_err, weight2.T)
        delta = h_err * relu_gradient(h_in)  # Apply ReLU gradient to hidden layer input
        gradient_in_h = np.dot(X_batch.T, delta)
        gradient_bias_h = np.sum(delta, axis=0, keepdims=True)

        # Update parameters
        weight1 -= learning_rate * gradient_in_h
        bias1 -= learning_rate * gradient_bias_h
        weight2 -= learning_rate * gradient_h
        bias2 -= learning_rate * gradient_bias_h

    # Evaluate on train
    all_train_in = np.dot(X_train, weight1) + bias1     
    all_h_train_out= relu_activation(all_train_in)
    all_in_train= np.dot(all_h_train_out, weight2) + bias2
    all_out_train= softmax_activation(all_in_train)

    train_loss = calculate_cross_entropy(Y_train, all_out_train)
    train_losses.append(train_loss)

    # Evaluate on validation
    all_val_in= np.dot(X_validation, weight1) + bias1
    all_h_val_out = relu_activation(all_val_in)
    all_in_val = np.dot(all_h_val_out, weight2) + bias2
    all_out_val = softmax_activation(all_in_val)

    val_acc = calculate_accuracy(y_validation, all_out_val)
    val_accuracies.append(val_acc)

    print(f"Epoch {epoch+1}/{epochs} - Loss: {train_loss:.4f} - Validation Accuracy: {val_acc:.4f}")

#  Final test evaluation
all_test_in = np.dot(X_test, weight1) + bias1
all_h_test_out = relu_activation(all_test_in)
all_in_test = np.dot(all_h_test_out, weight2) + bias2
all_out_test = softmax_activation(all_in_test)

test_acc = calculate_accuracy(y_test, all_out_test)
print(f"\nTest Accuracy: {test_acc * 100:.2f}%")

#  Plot results
plt.plot(train_losses)
plt.title("Training Loss")
plt.xlabel("Epoch")
plt.ylabel("Loss")
plt.grid(True)
plt.tight_layout()
plt.show()

plt.plot(val_accuracies)
plt.title("Validation Accuracy")
plt.xlabel("Epoch")
plt.ylabel("Accuracy")
plt.grid(True)
plt.tight_layout()
plt.show()