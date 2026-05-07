document.addEventListener("DOMContentLoaded", () => {

let cart = [];
let token = localStorage.getItem('token') || null;
let role = localStorage.getItem('role') || null;

const hostname = window.location.hostname || "localhost";
const API_BASE = `http://${hostname}:18080`;

function updateNavigation() {
  if (token) {
    document.getElementById('nav-login').style.display = 'none';
    document.getElementById('nav-logout').style.display = 'inline-block';
    
    if (role === 'shopkeeper') {
      document.getElementById('nav-shop').style.display = 'inline-block';
      document.getElementById('nav-cart').style.display = 'none'; // Optional: hide cart for shopkeepers
    } else {
      document.getElementById('nav-shop').style.display = 'none';
      document.getElementById('nav-cart').style.display = 'inline-block';
    }
  } else {
    document.getElementById('nav-login').style.display = 'inline-block';
    document.getElementById('nav-logout').style.display = 'none';
    document.getElementById('nav-shop').style.display = 'none';
    document.getElementById('nav-cart').style.display = 'inline-block';
  }
}

// PAGE SWITCH
function showPage(page) {
  document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
  document.getElementById(page).classList.add('active');

  document.querySelectorAll('.nav button').forEach(b => b.classList.remove('active-nav'));
  const activeBtn = document.getElementById('nav-' + page);
  if(activeBtn) activeBtn.classList.add('active-nav');

  if (page === 'cart') renderCart();
}
window.showPage = showPage;

window.logout = function() {
  localStorage.removeItem('token');
  localStorage.removeItem('role');
  token = null;
  role = null;
  updateNavigation();
  showPage('home');
}

// LOAD PRODUCTS
async function loadProducts() {
  try {
    const res = await fetch(`${API_BASE}/products`);
    const data = await res.json();
    displayProducts(data);
  } catch (err) {
    console.error(err);
    document.getElementById("products").innerHTML = "<p>Error loading products. Ensure backend is running!</p>";
  }
}

// DISPLAY PRODUCTS
function displayProducts(products) {
  const container = document.getElementById("products");
  const inventoryContainer = document.getElementById("shopkeeper-inventory");
  
  container.innerHTML = "";
  if (inventoryContainer) inventoryContainer.innerHTML = "";
  
  const currentUser = token ? token.split('_')[1] : null;

  products.forEach(p => {
    // For Shopkeeper: Add to inventory list if they own it
    if (role === 'shopkeeper' && p.owner === currentUser) {
      if (inventoryContainer) {
        const item = document.createElement("p");
        item.innerHTML = `
          <span><strong>${p.name}</strong> (SKU: ${p.id})</span>
          <span style="color: ${p.stock > 0 ? '#ff8a00' : 'red'};">Stock: ${p.stock}</span>
        `;
        inventoryContainer.appendChild(item);
      }
    }
    
    // For Customer: Display only if in stock
    if (role === 'customer' && p.stock > 0) {
      const card = document.createElement("div");
      card.className = "card";

      card.innerHTML = `
        <img src="${p.img}">
        <h3>${p.name}</h3>
        <p class="price">₹${p.price}</p>
        <p style="font-size: 12px; color: #aaa;">Stock: ${p.stock}</p>
        <button class="btn-primary">Add to Cart</button>
      `;

      card.querySelector("button").addEventListener("click", () => addToCart(p));
      container.appendChild(card);
    }
  });
  
  if (role === 'customer' && container.innerHTML === "") {
     container.innerHTML = "<p>No items currently available. Check back later!</p>";
  }
}

// ADD TO CART
function addToCart(product) {
  cart.push(product);
  document.getElementById("cartCount").innerText = cart.length;
}

// RENDER CART
function renderCart() {
  const container = document.getElementById("cartItems");
  const totalEl = document.getElementById("total");

  container.innerHTML = "";
  let total = 0;

  cart.forEach((p, index) => {
    total += p.price;

    const item = document.createElement("p");
    item.innerHTML = `
      <span>${p.name}</span>
      <span>₹${p.price} <button class="btn-primary" style="padding: 4px 8px; width:auto; margin-left:10px;" onclick="removeItem(${index})">X</button></span>
    `;

    container.appendChild(item);
  });

  totalEl.innerText = "Total: ₹" + total;
  window.cartTotal = total;
}

window.removeItem = function(index) {
  cart.splice(index, 1);
  document.getElementById("cartCount").innerText = cart.length;
  renderCart();
}

// CHECKOUT
window.checkout = async function() {
  if (!token) {
    alert("Please login first!");
    showPage('login');
    return;
  }
  if (cart.length === 0) {
    alert("Cart is empty!");
    return;
  }
  
  try {
    const skus = cart.map(p => p.id);
    const res = await fetch(`${API_BASE}/checkout`, {
      method: "POST",
      body: JSON.stringify({ token: token, total: window.cartTotal, skus: skus })
    });
    const data = await res.json();
    if (res.ok) {
      alert("Payment Successful! 🎉 Order ID: " + data.order_id);
      cart = [];
      document.getElementById("cartCount").innerText = 0;
      showPage('home');
    } else {
      alert("Checkout failed: " + data.error);
    }
  } catch(e) {
    alert("Network error during checkout.");
  }
}

// LOGIN / REGISTER API
window.doLogin = async function() {
  const u = document.getElementById('username').value;
  const p = document.getElementById('password').value;
  try {
    const res = await fetch(`${API_BASE}/login`, {
      method: "POST",
      body: JSON.stringify({ username: u, password: p })
    });
    const data = await res.json();
    if (res.ok) {
      token = data.token;
      role = data.role;
      localStorage.setItem('token', token);
      localStorage.setItem('role', role);
      updateNavigation();
      loadProducts(); // Refresh products to hide/show Add to Cart button based on role
      showPage('home');
    } else {
      alert("Login failed: " + data.error);
    }
  } catch(e) {
    alert("Error logging in. Is the backend running?");
  }
}

window.doRegister = async function() {
  const u = document.getElementById('username').value;
  const p = document.getElementById('password').value;
  const r = document.querySelector('input[name="role"]:checked').value;
  
  try {
    const res = await fetch(`${API_BASE}/register`, {
      method: "POST",
      body: JSON.stringify({ username: u, password: p, role: r })
    });
    if (res.ok) {
      alert("Registration successful! You can now log in.");
    } else {
      alert("Registration failed.");
    }
  } catch(e) {
    alert("Error registering. Is the backend running?");
  }
}

// ADD PRODUCT (SHOPKEEPER)
window.addProduct = async function() {
  const n = document.getElementById('prod-name').value;
  const p = parseFloat(document.getElementById('prod-price').value);
  
  if(!n || !p) {
    alert("Please fill name and price");
    return;
  }

  try {
    const res = await fetch(`${API_BASE}/products`, {
      method: "POST",
      body: JSON.stringify({ token: token, name: n, price: p })
    });
    const data = await res.json();
    if (res.ok) {
      alert("Product Listed! SKU: " + data.sku);
      document.getElementById('prod-name').value = '';
      document.getElementById('prod-price').value = '';
      loadProducts();
      showPage('home');
    } else {
      alert("Failed to add product: " + data.error);
    }
  } catch(e) {
    alert("Network error while adding product.");
  }
}

// SEARCH
document.getElementById("search").addEventListener("input", function() {
  const value = this.value.toLowerCase();
  document.querySelectorAll('.card').forEach(card => {
    const name = card.querySelector('h3').innerText.toLowerCase();
    card.style.display = name.includes(value) ? 'flex' : 'none';
  });
});

// INIT
updateNavigation();
if (!token) {
  showPage('login');
} else {
  loadProducts();
  showPage('home');
}

});
